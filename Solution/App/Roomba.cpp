#include "Roomba.h"
#include "define.h"
#include "Ball.h"
#include "AppCamera.h"
#include "AppStage.h"
#include "Crystal.h"
#include "Device.h"
#include <assert.h>

//デバッグのためスピード遅め
static const double SPEED = 0.1;
static const double MAX_SPEED = 0.2;
static const double CENTRIPETAL_POWER = 0.03;
static const double CENTRIPETAL_MIN = 7;
static const double MAX_SCALE = 25;
static const double MIN_SCALE = 8;

static const Vector START_POS[ 2 ] {
	( Vector( 15, 15 ) + Vector( STAGE_WIDTH_NUM, STAGE_HEIGHT_NUM ) ) * WORLD_SCALE,
	( Vector( 20, 20 ) + Vector( STAGE_WIDTH_NUM, STAGE_HEIGHT_NUM ) ) * WORLD_SCALE
};

Roomba::Roomba( ) :
_state( MOVE_STATE_NEUTRAL ),
_trans_speed( Vector( ) ),
_vec_trans( Vector( ) ),
_rot_speed( 0 ) {
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ] = BallPtr( new Ball( START_POS[ i ] ) );
		_vec_rot[ i ] = Vector( );
	}
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera ) {
	
	changeState( camera );

	updateState( );

	updateBalls( stage );

	holdCrystal( stage );

	shiftPos( );

}

void Roomba::updateState( ) {
	acceleration( );
	moveTranslation( );
	moveRotation( );
}

void Roomba::acceleration( ) {
	switch ( _state ) {
	case ACCEL_STATE_TRANSLATION:
		brakeRotation( );
		accelTranslation( );
		break;
	case STATE_ACCEL_ROTATION_RIGHT:
		brakeTranslation( );
		accelRotation( DIR_RIGHT );
		break;
	case STATE_ACCEL_ROTATION_LEFT:
		brakeTranslation( );
		accelRotation( DIR_LEFT );
		break;
	case MOVE_STATE_NEUTRAL:
		brakeTranslation( );
		brakeRotation( );
	default:
		assert( -1 );
		break;
	}
}

void Roomba::accelTranslation( ) {
	if ( _move_dir.x > 0 ) {
		_trans_speed.x += SPEED;
		if ( _trans_speed.x > MAX_SPEED ) {
			_trans_speed.x = MAX_SPEED;
		}
	} else if ( _move_dir.x < 0 ) {
		_trans_speed.x -= SPEED;
		if ( _trans_speed.x < -MAX_SPEED ) {
			_trans_speed.x = -MAX_SPEED;
		}
	}

	if ( _move_dir.y > 0 ) {
		_trans_speed.y += SPEED;
		if ( _trans_speed.y > MAX_SPEED ) {
			_trans_speed.y = MAX_SPEED;
		}
	} else if ( _move_dir.y < 0 ) {
		_trans_speed.y -= SPEED;
		if ( _trans_speed.y < -MAX_SPEED ) {
			_trans_speed.y = -MAX_SPEED;
		}
	}
}

void Roomba::accelRotation( DIR dir ) {
	if ( dir == DIR_LEFT ) {
		_rot_speed += SPEED;
		if ( _rot_speed > MAX_SPEED ) {
			_rot_speed = MAX_SPEED;
		}
	} else {
		_rot_speed += -SPEED;
		if ( _rot_speed < -MAX_SPEED ) {
			_rot_speed = -MAX_SPEED;
		}
	}
}

void Roomba::brakeTranslation( ) {
	if ( _trans_speed.x > 0 ) {
		_trans_speed.x -= SPEED;
		if ( _trans_speed.x < 0 ) {
			_trans_speed.x = 0;
		}
	} else {
		_trans_speed.x += SPEED;
		if ( _trans_speed.x > 0 ) {
			_trans_speed.x = 0;
		}
	}

	if ( _trans_speed.y > 0 ) {
		_trans_speed.y -= SPEED;
		if ( _trans_speed.y < 0 ) {
			_trans_speed.y = 0;
		}
	} else {
		_trans_speed.y += SPEED;
		if ( _trans_speed.y > 0 ) {
			_trans_speed.y = 0;
		}
	}
}

void Roomba::brakeRotation( ) {
	if ( _rot_speed < 0 ) {
		_rot_speed += SPEED;
		if ( _rot_speed > 0 ) {
			_rot_speed = 0;
		}
	} else {
		_rot_speed -= SPEED;
		if ( _rot_speed < 0 ) {
			_rot_speed = 0;
		}
	}
}

void Roomba::changeState( CameraPtr camera ) {
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	
	MOVE_STATE state = _state;
	_move_dir = Vector( );

	if ( right_stick.y > 0 && left_stick.y < 0 ) {
		state = STATE_ACCEL_ROTATION_RIGHT;
	}
	if ( right_stick.y < 0 && left_stick.y > 0 ) {
		state = STATE_ACCEL_ROTATION_LEFT;
	}
	if (  right_stick.y > 0 && left_stick.y > 0 ||
		  right_stick.y < 0 && left_stick.y < 0 ||
		  right_stick.x > 0 && left_stick.x > 0 ||
		  right_stick.x < 0 && left_stick.x < 0 ){
		state = ACCEL_STATE_TRANSLATION;
		Vector dir = ( ( right_stick + left_stick ) *= -1 ).normalize( );
		_move_dir = dir;
	}
	if ( right_stick == Vector( ) ||
		 left_stick == Vector( ) ) {
		state = MOVE_STATE_NEUTRAL;
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

void Roomba::moveTranslation( ) {
	Vector dir = _move_dir.normalize( ) * SPEED + _vec_trans.normalize( ) * _trans_speed.getLength( );
	Vector trans_vec = dir.normalize( ) * _trans_speed.getLength( );
	setVecTrans( trans_vec );
}

void Roomba::moveRotation( ) {
	std::array< Vector, 2 > vec;
	for ( int i = 0;  i < 2; i++ ) {
		Vector radius = _balls[ i ]->getPos( ) - getCentralPos( );
		Matrix mat_rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), _rot_speed / radius.getLength( ) );
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
	//drawer->drawLine( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
	const int ratio = 10; // effekseerのツールで作った際の大きさ
	double size = ( ( getCentralPos( ) - _balls[ BALL_LEFT ]->getPos( ) ).getLength( ) ) / ratio; // 大きさは左右どちらからでも変わらないため左を基準に取る
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

void Roomba::setVecTrans( Vector vec ) {
	_vec_trans = vec;
}

void Roomba::setVecRot( Vector vec_left, Vector vec_right ) {
	_vec_rot[ 0 ] = vec_left;
	_vec_rot[ 1 ] = vec_right;
}

void Roomba::updateBalls( StagePtr stage) {
	for ( int i = 0; i < 2; i++ ) {
		Vector vec = _vec_trans + _vec_rot[ i ];
		_balls[ i ]->update( vec, stage );
	}
}

void Roomba::shiftPos( ) {
	Vector pos[ 2 ];
	pos[ 0 ] = _balls[ 0 ]->getPos( );
	pos[ 1 ] = _balls[ 1 ]->getPos( );
	Vector central_pos =  getCentralPos( );
	if ( central_pos.x < STAGE_WIDTH_NUM * WORLD_SCALE - 1 ) {
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].x += STAGE_WIDTH_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
	if ( central_pos.y < STAGE_HEIGHT_NUM * WORLD_SCALE - 1 ) {
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].y += STAGE_HEIGHT_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
}
