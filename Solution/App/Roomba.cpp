#include "Roomba.h"
#include "define.h"
#include "Ball.h"
#include "AppCamera.h"
#include "AppStage.h"
#include "Crystal.h"
#include "Device.h"
#include <assert.h>

static const double SPEED = 0.18;
static const double MAX_SPEED = 0.7;
static const double CENTRIPETAL_POWER = 0.03;
static const double CENTRIPETAL_MIN = 7;
static const double MAX_SCALE = 25;
static const double MIN_SCALE = 8;


static const Vector START_POS[ 2 ] {
	Vector( 15, 15 ) * WORLD_SCALE,
	Vector( 20, 20 ) * WORLD_SCALE
};

Roomba::Roomba( ) :
_state( MOVE_STATE_TRANSLATION ),
_trans_speed( 0 ),
_rot_speed( 0 ) {
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ] = BallPtr( new Ball( START_POS[ i ] ) );
		_vec_trans[ i ] = Vector( );
		_vec_rot[ i ] = Vector( );
	}
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera ) {
	updateState( camera );
	move( camera );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->setForce( _vec_trans[ i ] + _vec_rot[ i ] );
		if ( _balls[ i ]->getVec( ).getLength2( ) > MAX_SPEED * MAX_SPEED ) {
			_balls[ i ]->setForce( _balls[ i ]->getVec( ).normalize( ) * MAX_SPEED );
		}

		_balls[ i ]->update( stage );
	}
	holdCrystal( stage );

	//ステージ端
	Vector pos[ 2 ];
	pos[ 0 ] = _balls[ 0 ]->getPos( );
	pos[ 1 ] = _balls[ 1 ]->getPos( );
	Vector central_pos =  getCentralPos( );
	if ( central_pos.x < 0 ) {
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].x += STAGE_WIDTH_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
	if ( central_pos.x > STAGE_WIDTH_NUM * WORLD_SCALE - 1 ) {
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].x -= STAGE_WIDTH_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
	if ( central_pos.y < 0  ) {
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].y += STAGE_HEIGHT_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
	if ( central_pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE - 1 ) {
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
}

void Roomba::move( CameraPtr camera ) {
	Vector camera_dir = camera->getDir( );
	camera_dir.z = 0;
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );

	acceleration( right_stick, left_stick );

	moveTranslation( camera_dir, right_stick, left_stick );

	switch ( _state ) {
	case MOVE_STATE_ROTATION_RIGHT:		
		moveRotation( -1 );
		break;
	case MOVE_STATE_ROTATION_LEFT:
		moveRotation( 1 );
		break;
	}

}

void Roomba::acceleration( Vector right_stick, Vector left_stick ) {
	if ( right_stick != Vector( ) ||
		 left_stick != Vector( ) ) {
		// 加速
		switch ( _state ) {
		case MOVE_STATE_TRANSLATION:
			_trans_speed += SPEED;
			if ( _trans_speed > MAX_SPEED ) {
				_trans_speed = MAX_SPEED;
			}
			_rot_speed -= SPEED;
			if ( _rot_speed < 0 ) {
				_rot_speed = 0;
			}
			break;
		case MOVE_STATE_ROTATION_LEFT:
		case MOVE_STATE_ROTATION_RIGHT:
			_rot_speed += SPEED;
			if ( _rot_speed > MAX_SPEED ) {
				_rot_speed = MAX_SPEED;
			}
			_trans_speed -= SPEED;
			if ( _trans_speed < 0 ) {
				_trans_speed = 0;
			}
			break;
		}
	} else {
		// 減速
		_trans_speed -= SPEED;
		if ( _trans_speed < 0 ) {
			_trans_speed = 0;
		}
		_rot_speed -= SPEED;
		if ( _rot_speed < 0 ) {
			_rot_speed = 0;
		}
	}
}

void Roomba::updateState( CameraPtr camera ) {
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	
	MOVE_STATE state = _state;
	if ( right_stick.y > 0  && left_stick.y < 0 ) {
		state = MOVE_STATE_ROTATION_RIGHT;
	}
	if ( right_stick.y < 0  && left_stick.y > 0 ) {
		state = MOVE_STATE_ROTATION_LEFT;
	}
	if ( ( right_stick.x < 0 && left_stick.x < 0 ) ||
		 ( right_stick.x > 0 && left_stick.x > 0 ) ||
		 ( right_stick.y < 0 && left_stick.y < 0 ) ||
		 ( right_stick.y > 0 && left_stick.y > 0 ) ||
		 ( right_stick.x > 0 && left_stick.x < 0 ) ||
		 ( right_stick.x < 0 && left_stick.x > 0 ) ) {
		state = MOVE_STATE_TRANSLATION;
	}
	if ( state != _state ) {
		_state = state;
		checkLeftRight( camera );
	}
}

void Roomba::holdCrystal( StagePtr stage ) {
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( stage );
	if ( !_crystal ) {
		_crystal =  app_stage->getHittingCrystal( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
	}
	if ( _crystal != CrystalPtr( ) ) {
		if ( _crystal->isDropDown( ) || _crystal->isFinished( ) ) {
			_crystal = CrystalPtr( );
			return;
		}
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 300, 20, "もってるよー" );
		Vector central_pos = getCentralPos( );
		Vector crystal_pos = _crystal->getPos( );
		Vector distance = central_pos - crystal_pos;
		if ( distance.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
			central_pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
		}
		if ( -distance.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
			central_pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
		}
		if ( distance.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
			central_pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
		}
		if ( -distance.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
			central_pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
		}
		_crystal->setVec( central_pos - crystal_pos );
	}
}

void Roomba::moveTranslation( const Vector& camera_dir, const Vector& right, const Vector& left ) {
	Matrix mat = Matrix::makeTransformRotation( camera_dir.cross( Vector( 0, -1 ) ), camera_dir.angle( Vector( 0, -1 ) ) );
	
	Vector dir_left  = mat.multiply( left  ).normalize( ) + _vec_trans[ 0 ].normalize( );
	Vector dir_right = mat.multiply( right ).normalize( ) + _vec_trans[ 1 ].normalize( );
	Vector vec_left  = dir_left.normalize( ) * _trans_speed;
	Vector vec_right = dir_right.normalize( ) * _trans_speed;
	Vector scale_left = Vector( );
	Vector scale_right = Vector( );

	// translationとscaleに分解
	if ( fabs( vec_left.x ) > fabs( vec_left.y ) ) {
		scale_left.y = vec_left.y;
		vec_left.y = 0;
	} else {
		scale_left.x = vec_left.x;
		vec_left.x = 0;
	}
	if ( fabs( vec_right.x ) > fabs( vec_right.y ) ) {
		scale_right.y = vec_right.y;
		vec_right.y = 0;
	} else {
		scale_right.x = vec_right.x;
		vec_right.x = 0;
	}

	// scale移動判定
	Vector scale = _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( );
	if ( ( scale_left + scale_right + scale ).getLength( ) > MAX_SCALE ) { 
		Vector left_dir = _balls[ BALL_RIGHT ]->getPos( ) - _balls[ BALL_LEFT ]->getPos( );
		Vector right_dir = _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( );
		scale_left = left_dir.normalize( );
		scale_right = right_dir.normalize( );
	}
	if ( ( scale_left + scale_right + scale ).getLength( ) < MIN_SCALE ) { 
		Vector left_dir = _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( );
		Vector right_dir = _balls[ BALL_RIGHT ]->getPos( ) - _balls[ BALL_LEFT ]->getPos( );
		scale_left = left_dir.normalize( ) * ( ( scale_left + scale_right + scale ).getLength( ) * 0.5 );
		scale_right = right_dir.normalize( ) * ( ( scale_left + scale_right + scale ).getLength( ) * 0.5 );
	}
	setVecTrans( vec_left + scale_left, vec_right + scale_right );
}

void Roomba::moveRotation( int rotation_dir ) {
	std::array< Vector, 2 > vec;
	for ( int i = 0;  i < 2; i++ ) {
		Vector radius = _balls[ i ]->getPos( ) - getCentralPos( );
		Matrix mat_rot = Matrix::makeTransformRotation( Vector( 0, 0, rotation_dir ), _rot_speed / radius.getLength( ) );
		Vector radius2 = mat_rot.multiply( radius );
		vec[ i ] = ( ( radius2 + getCentralPos( ) ) - _balls[ i ]->getPos( ) );
	}
	setVecRot( vec[ 0 ], vec[ 1 ] );
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->draw( );
	}
	// レーザー
	drawer->drawLine( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
	double size = ( ( getCentralPos( ) - _balls[ BALL_LEFT ]->getPos( ) ).getLength( ) ) / 10; // 大きさは左右どちらからでも変わらないため左を基準に取る
	double angle_left = ( getCentralPos( ) - _balls[ BALL_LEFT ]->getPos( ) ).angle( Vector( 1, 0 ) );
	if ( ( getCentralPos( ) - _balls[ BALL_LEFT ]->getPos( ) ).cross( Vector( 1, 0 ) ).z == 1 ) {
		angle_left = PI2 - angle_left;
	}
	drawer->setEffect( Drawer::Effect( EFFECT_LASER, _balls[ BALL_LEFT ]->getPos( ), size, Vector( 0, 0, angle_left ) ) );

	double angle_right = ( getCentralPos( ) - _balls[ BALL_RIGHT ]->getPos( ) ).angle( Vector( 1, 0 ) );
	if ( ( getCentralPos( ) - _balls[ BALL_RIGHT ]->getPos( ) ).cross( Vector( 1, 0 ) ).z == 1 ) {
		angle_right = PI2 - angle_right;
	}
	drawer->setEffect( Drawer::Effect( EFFECT_LASER, _balls[ BALL_RIGHT ]->getPos( ), size, Vector( 0, 0, angle_right ) ) );
}

Vector Roomba::getCentralPos( ) const {
	Vector pos[ 2 ];
	for ( int i = 0; i < 2; i++ ) {
		pos[ i ] = _balls[ i ]->getPos( );
	}
	Vector central_pos = ( pos[ 0 ] + pos[ 1 ] ) * 0.5;

	return central_pos;
}

void Roomba::reset( ) {
	_balls[ BALL_LEFT ]->reset( START_POS[ 0 ] );
	_balls[ BALL_RIGHT ]->reset( START_POS[ 1 ] );
	_crystal = CrystalPtr( );
}

void Roomba::checkLeftRight( CameraPtr camera ) {	
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector vec = mat.multiply( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) );
	double dot = camera->getDir( ).dot( vec );
	if ( dot > 0 ) {
		BallPtr tmp = _balls[ BALL_LEFT ];
		_balls[ BALL_LEFT ] = _balls[ BALL_RIGHT ];
		_balls[ BALL_RIGHT ] = tmp;
	}
}

Vector Roomba::getBallPos( int ball ) const {
	return _balls[ ball ]->getPos( );
}

void Roomba::setVecTrans( Vector vec_left, Vector vec_right ) {
	_vec_trans[ 0 ] = vec_left;
	_vec_trans[ 1 ] = vec_right;
}

void Roomba::setVecRot( Vector vec_left, Vector vec_right ) {
	_vec_rot[ 0 ] = vec_left;
	_vec_rot[ 1 ] = vec_right;
}
