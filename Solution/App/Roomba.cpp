#include "Roomba.h"
#include "define.h"
#include "Ball.h"
#include "AppCamera.h"
#include "AppStage.h"
#include "Crystal.h"
#include "Device.h"
#include <assert.h>

//デバッグのためスピード遅め
//static const double SPEED = 0.1;
static const double ACCEL_SPEED = 0.03;
static const double RESTORE_SPEED = 0.08;
static const double MAX_ROT_SPEED = 0.2;
static const double MAX_TRANS_SPEED = 0.4;
static const double DECELETION_ROT_SPEED = 0.03;
static const double DECELETION_TRANS_SPEED = 0.0003;
static const double EMERGENCY_DECELERATION_SPEED = 0.05;
static const double SCALE_SIZE = 6;
static const double MIN_SCALE = 5;

static const Vector START_POS[ 2 ] {
	( Vector( STAGE_WIDTH_NUM + 17, STAGE_HEIGHT_NUM + 3 ) * WORLD_SCALE + Vector( 0, 0, roomba_size.z ) ),
	( Vector( STAGE_WIDTH_NUM + 20, STAGE_HEIGHT_NUM + 3 ) * WORLD_SCALE + Vector( 0, 0, roomba_size.z ) )
};

Roomba::Roomba( ) :
_state( MOVE_STATE_NEUTRAL ),
_trans_speed( Vector( ) ),
_vec_trans( Vector( ) ),
_rot_speed( 0 ),
_scaling( false ) {
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ] = BallPtr( new Ball( START_POS[ i ] ) );
		_vec_rot[ i ] = Vector( );
		_vec_scale[ i ] = Vector( );
		_vec_reflection[ i ] = Vector( );
	}
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera ) {
	
	changeState( camera );

	updateState( );

	holdCrystal( stage );

	updateBalls( stage );

	shiftPos( camera );

}

void Roomba::updateState( ) {
	acceleration( );
	moveTranslation( );
	moveRotation( );
	moveReflection( );
	moveRestore( );
}

void Roomba::acceleration( ) {
	switch ( _state ) {
	case MOVE_STATE_TRANSLATION:
		brakeRotation( );
		accelTranslation( );
		break;
	case MOVE_STATE_ROTATION_RIGHT:
		brakeTranslation( );
		accelRotation( DIR_RIGHT );
		break;
	case MOVE_STATE_ROTATION_LEFT:
		brakeTranslation( );
		accelRotation( DIR_LEFT );
		break;
	case MOVE_STATE_NEUTRAL:
	case MOVE_STATE_RESTORE:
		brakeTranslation( );
		brakeRotation( );
	default:
		assert( -1 );
		break;
	}
}

void Roomba::accelTranslation( ) {
	_trans_speed.x += _move_dir.x * ACCEL_SPEED;
	_trans_speed.y += _move_dir.y * ACCEL_SPEED;
	if ( _move_dir.x > 0 ) {
		if ( _trans_speed.x > MAX_TRANS_SPEED ) {
			_trans_speed.x = MAX_TRANS_SPEED;
		}
	} else if ( _move_dir.x < 0 ) {
		if ( _trans_speed.x < -MAX_TRANS_SPEED ) {
			_trans_speed.x = -MAX_TRANS_SPEED;
		}
	}

	if ( _move_dir.y > 0 ) {
		if ( _trans_speed.y > MAX_TRANS_SPEED ) {
			_trans_speed.y = MAX_TRANS_SPEED;
		}
	} else if ( _move_dir.y < 0 ) {
		if ( _trans_speed.y < -MAX_TRANS_SPEED ) {
			_trans_speed.y = -MAX_TRANS_SPEED;
		}
	}
}

void Roomba::accelRotation( DIR dir ) {
	if ( dir == DIR_LEFT ) {
		_rot_speed += _move_dir.y * ACCEL_SPEED;
		if ( _rot_speed > MAX_ROT_SPEED ) {
			_rot_speed = MAX_ROT_SPEED;
		}
	} else {
		_rot_speed += _move_dir.y * ACCEL_SPEED;
		if ( _rot_speed < -MAX_ROT_SPEED ) {
			_rot_speed = -MAX_ROT_SPEED;
		}
	}
}

void Roomba::brakeTranslation( ) {
	double deceletion_trans_speed_x = DECELETION_TRANS_SPEED;
	double deceletion_trans_speed_y = DECELETION_TRANS_SPEED;
	if ( fabs( _move_dir.x ) < 0.1 && fabs( _move_dir.y ) > 0.1 ) {
		deceletion_trans_speed_x = ACCEL_SPEED / 2;
	}
	if ( fabs( _move_dir.y ) < 0.1 && fabs( _move_dir.x ) > 0.1 ) {
		deceletion_trans_speed_y = ACCEL_SPEED / 2;
	}
	if ( _trans_speed.x > 0 ) {
		_trans_speed.x -= deceletion_trans_speed_x;
		if ( _trans_speed.x < 0 ) {
			_trans_speed.x = 0;
		}
	} else {
		_trans_speed.x += deceletion_trans_speed_x;
		if ( _trans_speed.x > 0 ) {
			_trans_speed.x = 0;
		}
	}

	if ( _trans_speed.y > 0 ) {
		_trans_speed.y -= deceletion_trans_speed_y;
		if ( _trans_speed.y < 0 ) {
			_trans_speed.y = 0;
		}
	} else {
		_trans_speed.y += deceletion_trans_speed_y;
		if ( _trans_speed.y > 0 ) {
			_trans_speed.y = 0;
		}
	}
}

void Roomba::brakeRotation( ) {
	if ( _rot_speed < 0 ) {
		_rot_speed += DECELETION_ROT_SPEED;
		if ( _rot_speed > 0 ) {
			_rot_speed = 0;
		}
	} else {
		_rot_speed -= DECELETION_ROT_SPEED;
		if ( _rot_speed < 0 ) {
			_rot_speed = 0;
		}
	}
}

void Roomba::changeState( CameraPtr camera ) {
	DevicePtr device = Device::getTask( );
	Vector right_stick = Vector( device->getRightDirX( ), device->getRightDirY( ) ) * 0.002;
	Vector left_stick = Vector( device->getDirX( ), device->getDirY( ) ) * 0.002;

	MOVE_STATE state = _state;
	_move_dir = Vector( );
	if ( right_stick.y > 0 && left_stick.y < 0 ) {
		state = MOVE_STATE_ROTATION_RIGHT;
		_move_dir = left_stick - right_stick;
	}
	if ( right_stick.y < 0 && left_stick.y > 0 ) {
		state = MOVE_STATE_ROTATION_LEFT;
		_move_dir = left_stick - right_stick;
	}
	if (  right_stick.y > 0 && left_stick.y > 0 ||
		  right_stick.y < 0 && left_stick.y < 0 ||
		  right_stick.x > 0 && left_stick.x > 0 ||
		  right_stick.x < 0 && left_stick.x < 0 ){
		state = MOVE_STATE_TRANSLATION;
		_move_dir = _stick_rot.multiply( right_stick + left_stick );
	}
	if ( right_stick == Vector( ) ||
		 left_stick == Vector( ) ) {
		state = MOVE_STATE_NEUTRAL;
	}
	if ( _balls[ 0 ]->isReflection( ) ||
		 _balls[ 1 ]->isReflection( ) ) {
		state = MOVE_STATE_REFLECTION;
	}
	if ( _state == MOVE_STATE_REFLECTION ) {
		if ( ( _balls[ 0 ]->getVec( ) == Vector( ) )  &&
			 ( _balls[ 1 ]->getVec( ) == Vector( ) ) ) {
			state = MOVE_STATE_RESTORE;
		}
	}
	if ( _state == MOVE_STATE_RESTORE ) {
		state = MOVE_STATE_RESTORE;
	}
	double scale = ( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) ).getLength( );
	if ( state == MOVE_STATE_RESTORE ) {
		if ( scale > MIN_SCALE && scale < SCALE_SIZE ) {
			state = MOVE_STATE_NEUTRAL;
		}
	}
	if ( state != _state ) {			
		if ( state == MOVE_STATE_TRANSLATION ) {
			Vector dir = camera->getDir( );
			dir.z = 0;
			_stick_rot = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( dir ) * -1, Vector( 0, -1 ).angle( dir ) );
		}
		if ( state == MOVE_STATE_REFLECTION ) {
			setVecScale( Vector( ), Vector( ) );
			_trans_speed = Vector( );
			_rot_speed = 0;
			_vec_reflection[ 0 ] = _balls[ 0 ]->getVec( );
			_vec_reflection[ 1 ] = _balls[ 1 ]->getVec( );
		}
		if ( state == MOVE_STATE_RESTORE ) {
			_balls[ 0 ]->setReflection( false );
			_balls[ 1 ]->setReflection( false );
		}
		if ( _state == MOVE_STATE_RESTORE ) {
			setVecScale( Vector( ), Vector( ) );
		}
		_state = state;
	}
}

void Roomba::holdCrystal( StagePtr stage ) {
	if ( _crystal != CrystalPtr( ) ) {
		if ( _crystal->isDropDown( ) ||
			 _crystal->isFinished( ) ||
			 _state == MOVE_STATE_REFLECTION ) {
			_crystal->setDropDown( false );
			_crystal = CrystalPtr( );
			return;
		}
		Vector crystal_pos = _crystal->getPos( );
		Vector pos[ 2 ] = {
			_balls[ 0 ]->getPos( ) + _balls[ 0 ]->getVec( ),
			_balls[ 1 ]->getPos( ) + _balls[ 1 ]->getVec( )
		};
		crystal_pos.z = pos[ 0 ].z;
		Vector line = pos[ 1 ] - pos[ 0 ];
		pos[ 0 ] += line.normalize( ) * BALL_RADIUS * 3;
		pos[ 1 ] -= line.normalize( ) * BALL_RADIUS * 3;
		line = pos[ 1 ] - pos[ 0 ];
		Vector to_crystal = pos[ 0 ] - crystal_pos;
		double angle = line.angle( to_crystal );
		double check = to_crystal.getLength( ) * cosl( angle );
		if ( check > 0 ) {							//線より外側
			_crystal->setVec( pos[ 0 ] - crystal_pos );
		} else if ( check < -line.getLength( ) ) {	//線より外側
			_crystal->setVec( pos[ 1 ] - crystal_pos );
		} else {									//線に垂直
			double distance = to_crystal.getLength( ) * fabs( sinl( angle ) );
			Matrix rot = Matrix::makeTransformRotation( line.cross( to_crystal ) * -1, PI / 2 );
			Vector vec = rot.multiply( line );
			vec = vec.normalize( ) * distance;
			_crystal->setVec( vec );
		}
	}
}

void Roomba::moveTranslation( ) {
	setVecTrans( _trans_speed );
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

void Roomba::moveReflection( ) {
	if ( _state != MOVE_STATE_REFLECTION ) {
		return;
	}
	for ( int i = 0; i < 2; i++ ) {
		if ( _vec_reflection[ i ].getLength( ) > EMERGENCY_DECELERATION_SPEED ) {
			_vec_reflection[ i ] -= _vec_reflection[ i ].normalize( ) * EMERGENCY_DECELERATION_SPEED;
		} else {
			_vec_reflection[ i ] = Vector( );
		}
	}
}
void Roomba::moveRestore( ) {
	if ( _state != MOVE_STATE_RESTORE ) {
		_scaling = false;
		setVecScale( Vector( ), Vector( ) );
		return;
	}
	
	// ball_leftを基準に縮小する
	_scaling = true;
	Vector dir = _balls[ 1 ]->getPos( ) - _balls[ 0 ]->getPos( );
	Vector vec = dir.normalize( ) * RESTORE_SPEED;
	double scale = ( _balls[ 1 ]->getPos( ) - _balls[ 0 ]->getPos( ) ).getLength( );
	if ( scale > SCALE_SIZE ) {
		setVecScale( vec, vec * -1 );
	}
	if ( scale < MIN_SCALE ) {
		setVecScale( vec * -1, vec );
	}
	
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->draw( );
	}
	if ( _state != MOVE_STATE_RESTORE &&
		 _state != MOVE_STATE_REFLECTION ) {
		drawLaser( );
	}
}

void Roomba::drawLaser( ) const {
	// レーザー
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawLine( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
#if 0

#else
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
#endif
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
	_trans_speed = Vector( );
	_vec_trans = Vector( );
	_rot_speed = 0;
	for ( int i = 0; i < 2; i++ ) {
		_vec_rot[ i ] = Vector( );
		_vec_scale[ i ] = Vector( );
		_vec_reflection[ i ] = Vector( );
	}
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

void Roomba::setVecScale( Vector vec_left, Vector vec_right ) {
	_vec_scale[ 0 ] = vec_left;
	_vec_scale[ 1 ] = vec_right;
}

void Roomba::setVecReflection( Vector vec_left, Vector vec_right ) {
	_vec_reflection[ 0 ] = vec_left;
	_vec_reflection[ 1 ] = vec_right;
}

void Roomba::updateBalls( StagePtr stage) {
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( stage );
	Vector vec[ 2 ];
	for ( int i = 0; i < 2; i++ ) {
		if ( _state == MOVE_STATE_RESTORE ||
			 _state == MOVE_STATE_REFLECTION ) {
			vec[ i ] = _vec_reflection[ i ] + _vec_scale[ i ];
		} else {
			vec[ i ] = _vec_trans + _vec_rot[ i ];
		}
	}

	if ( !_crystal ) {
		_crystal =  app_stage->getHittingCrystal( _balls[ 0 ]->getPos( ), _balls[ 1 ]->getPos( ), vec[ 0 ], vec[ 1 ] );
	}
	_balls[ 0 ]->update( vec[ 0 ], stage );
	_balls[ 1 ]->update( vec[ 1 ], stage );
}

void Roomba::shiftPos( CameraPtr camera ) {
	Vector pos[ 2 ];
	pos[ 0 ] = _balls[ 0 ]->getPos( );
	pos[ 1 ] = _balls[ 1 ]->getPos( );
	Vector central_pos =  getCentralPos( );
	bool shift = false;
	if ( central_pos.x < STAGE_WIDTH_NUM * WORLD_SCALE - 1 ) {
		shift = true;
		central_pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].x += STAGE_WIDTH_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
	if ( central_pos.y < STAGE_HEIGHT_NUM * WORLD_SCALE - 1 ) {
		shift = true;
		central_pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].y += STAGE_HEIGHT_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
	AppCameraPtr app_camera = std::dynamic_pointer_cast< AppCamera >( camera );
	if ( shift ) {
		if ( _crystal ) {
			_crystal->shiftPos( central_pos );
		}
		app_camera->shiftPos( central_pos );
	}
}

Vector Roomba::getDir( ) const {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector roomba_dir = mat.multiply( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) );
	return roomba_dir.normalize( );
}

bool Roomba::isScaling( ) const {
	return _scaling;
}

Matrix Roomba::getMat( const int ball_num, const Vector& pos ) const {
	if ( ball_num != 0 &&
		 ball_num != 1 ) {
		return Matrix( );
		}
	return _balls[ ball_num ]->getMat( pos );
}