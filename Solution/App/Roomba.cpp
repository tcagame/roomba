#include "Roomba.h"
#include "define.h"
#include "Ball.h"
#include "AppCamera.h"
#include "AppStage.h"
#include "Crystal.h"
#include "Device.h"
#include "laser.h"
#include <assert.h>
#include  "Sound.h"

//デバッグのためスピード遅め
//static const double SPEED = 0.1;
const double ACCEL_SPEED = 0.03;
const double RESTORE_SPEED = 0.08;
const double MAX_ROT_SPEED = 0.2;
const double MAX_TRANS_SPEED = 0.4;
const double DECELETION_ROT_SPEED = 0.002;
const double DECELETION_TRANS_SPEED = 0.001;
const double OTHER_TRANS_RATIO = 6;
const double OTHER_ROT_RATIO = 2;
const double EMERGENCY_DECELERATION_SPEED = 0.1;
const double SCALE_SIZE = 6;
const double MIN_SCALE = 5;
const double LIFT_Z = 10;
const double DELIVERY_FOOT = 2.5;
const int MAX_LINK_GAUGE = 400;
const double LINK_REDUCED_SPEED = 1.5;
const double LINK_RECOVERS_SPEED = 4.0;
const double BOUND_POW = 0.6;

static const Vector START_POS[ 2 ] {
	( Vector( STAGE_WIDTH_NUM + 19, STAGE_HEIGHT_NUM + 3 ) * WORLD_SCALE + Vector( 0, 0, ROOMBA_SIZE.z ) ),
	( Vector( STAGE_WIDTH_NUM + 22, STAGE_HEIGHT_NUM + 3 ) * WORLD_SCALE + Vector( 0, 0, ROOMBA_SIZE.z ) )
};

Roomba::Roomba( ) :
_state( MOVE_STATE_NEUTRAL ),
_trans_speed( Vector( ) ),
_vec_trans( Vector( ) ),
_rot_speed( 0 ),
_link_break( false ),
_link_gauge( MAX_LINK_GAUGE / 2 ) {
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ] = BallPtr( new Ball( START_POS[ i ] ) );
		_vec_rot[ i ] = Vector( );
		_vec_scale[ i ] = Vector( );
		_vec_reflection[ i ] = Vector( );
		_delivery[ i ].type = MDL_DELIVERY;
	}
	_laser = LaserPtr( new Laser );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera ) {
	changeState( camera );
	updateState( );
	holdCrystal( stage );
	updateBalls( stage );
	shiftPos( camera );
	updateLink( );
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	_laser->draw( );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->draw( );
		if ( _state == MOVE_STATE_LIFT_UP ||
			 _state == MOVE_STATE_LIFT_DOWN ) {
			drawer->setModelMDL( _delivery[ i ] );
		}
	}
}
/*
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
*/

void Roomba::updateState( ) {
	acceleration( );
	moveTranslation( );
	moveRotation( );
	moveReflection( );
	moveLiftUp( );
	moveLiftDown( );
}

void Roomba::updateLaser( CameraConstPtr camera ) {
	_laser->show( _state != MOVE_STATE_REFLECTION && _state != MOVE_STATE_REFLECTION_RESTORE );
	_laser->update( getCentralPos( ), camera, _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ), _crystal );
}

void Roomba::updateLink( ) {
	if ( _state != MOVE_STATE_REFLECTION &&
		 _state != MOVE_STATE_REFLECTION_RESTORE ) {
		_link_gauge += LINK_RECOVERS_SPEED;
		if ( _link_gauge > MAX_LINK_GAUGE ) {
			_link_gauge = MAX_LINK_GAUGE;
		}
	} else {
		_link_gauge -= LINK_REDUCED_SPEED;
		if ( _link_gauge <= 0 ) {
			_link_gauge = 0;
			_link_break = true;
		}
	}
}

void Roomba::updateBalls( StagePtr stage) {
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( stage );
	Vector vec[ 2 ];
	for ( int i = 0; i < 2; i++ ) {
		if ( _state == MOVE_STATE_REFLECTION ||
			 _state == MOVE_STATE_REFLECTION_RESTORE ) {
			vec[ i ] = _vec_reflection[ i ] + _vec_scale[ i ];
		} else if (
			_state == MOVE_STATE_LIFT_DOWN ||
			_state == MOVE_STATE_LIFT_UP ) {
			vec[ i ] = _vec_lift[ i ];
		} else {
			vec[ i ] = _vec_trans + _vec_rot[ i ];
		}
	}

	if ( !_crystal ) {
		_crystal =  app_stage->getHittingCrystal( _balls[ 0 ]->getPos( ), _balls[ 1 ]->getPos( ), vec[ 0 ], vec[ 1 ] );
		if ( _crystal ) {
			_crystal->setDropDown( false );
		}
	}
	_balls[ 0 ]->update( vec[ 0 ], stage );
	_balls[ 1 ]->update( vec[ 1 ], stage );
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
	double scale = ( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) ).getLength( );

	if ( _balls[ 0 ]->isReflection( ) ||
		 _balls[ 1 ]->isReflection( ) ) {
		state = MOVE_STATE_REFLECTION;
	}
	if ( _state == MOVE_STATE_REFLECTION ) {
		if ( ( _vec_reflection[ 0 ] == Vector( ) )  &&
			 ( _vec_reflection[ 1 ] == Vector( ) ) ) {
			state = MOVE_STATE_REFLECTION_RESTORE;
		}
	}
	if ( _state == MOVE_STATE_REFLECTION_RESTORE ) {
		state = MOVE_STATE_REFLECTION_RESTORE;
		if ( scale > MIN_SCALE && scale < SCALE_SIZE ) {
			state = MOVE_STATE_NEUTRAL;
		}
	}

	if ( _link_break ) {
		if ( _balls[ 0 ]->getPos( ).z < LIFT_Z ) {
			state = MOVE_STATE_LIFT_UP;
		} else {
			state = MOVE_STATE_LIFT_DOWN;
		}
		if ( _state == MOVE_STATE_LIFT_DOWN ) {
			state = MOVE_STATE_LIFT_DOWN;
			SoundPtr sound = Sound::getTask( );
			//sound->playSE( "se_maoudamashii_effect14.wav" );
		}
	}

	if ( state != _state ) {
		//ステート変更がある場合の処理
		if ( state == MOVE_STATE_TRANSLATION ) {
			Vector dir = camera->getDir( );
			dir.z = 0;
			_stick_rot = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( dir ) * -1, Vector( 0, -1 ).angle( dir ) );
			_move_dir = _stick_rot.multiply( right_stick + left_stick );
		}
		if ( state == MOVE_STATE_REFLECTION ) {
			SoundPtr sound = Sound::getTask( );
			sound->playSE( "knocking_a_wall.mp3" );
			_trans_speed = Vector( );
			_rot_speed = 0;
			for ( int i = 0; i < 2; i++ ) {
				_vec_reflection[ i ] = _balls[ i ]->getVec( );
				if ( _balls[ i ]->isReflection( ) ) {
					if ( !( _vec_reflection[ i ].z > 0 ) ) {
						_vec_reflection[ i ].z = BOUND_POW;
					}
				}
			}
		}
		if ( state == MOVE_STATE_LIFT_UP ) {
			_delivery[ 0 ].pos = _balls[ 0 ]->getPos( ) + Vector( 0, 0, LIFT_Z );
			_delivery[ 1 ].pos = _balls[ 1 ]->getPos( ) + Vector( 0, 0, LIFT_Z );
			_balls[ 0 ]->setReflection( false );
			_balls[ 1 ]->setReflection( false );
			_crystal = CrystalPtr( );
		}

		if ( _state == MOVE_STATE_REFLECTION ) {
			_balls[ 0 ]->setReflection( false );
			_balls[ 1 ]->setReflection( false );
		}
		if ( _state == MOVE_STATE_LIFT_UP ) {
			_vec_lift[ 0 ] = Vector( );
			_vec_lift[ 1 ] = Vector( );
			for ( int i = 0; i < 2; i++ ) {
				Vector ball = START_POS[ i ] + Vector( 0, 0, LIFT_Z );
				_balls[ i ]->setPos( ball );
				_delivery[ i ].pos = ball + Vector( 0, 0, DELIVERY_FOOT );
			}
		}
		drawEffect( state );
		_state = state;
	}
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
		brakeTranslation( );
		brakeRotation( );
		break;
	case MOVE_STATE_REFLECTION:
	case MOVE_STATE_REFLECTION_RESTORE:
		break;
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
	_rot_speed += _move_dir.y * ACCEL_SPEED;
	if ( _rot_speed > MAX_ROT_SPEED ) {
		_rot_speed = MAX_ROT_SPEED;
	}
	if ( _rot_speed < -MAX_ROT_SPEED ) {
		_rot_speed = -MAX_ROT_SPEED;
	}	
}

void Roomba::brakeTranslation( ) {
	double deceletion_trans_speed_x = DECELETION_TRANS_SPEED;
	double deceletion_trans_speed_y = DECELETION_TRANS_SPEED;
	if ( _state != MOVE_STATE_TRANSLATION ) {
		deceletion_trans_speed_x *= OTHER_TRANS_RATIO;
		deceletion_trans_speed_y *= OTHER_TRANS_RATIO;
	}
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
	double deceletion_speed = DECELETION_ROT_SPEED;
	if ( _state != MOVE_STATE_ROTATION_LEFT &&
		 _state != MOVE_STATE_ROTATION_RIGHT ) {
		deceletion_speed *= OTHER_ROT_RATIO;
	}
	if ( _rot_speed < 0 ) {
		_rot_speed += deceletion_speed;
		if ( _rot_speed > 0 ) {
			_rot_speed = 0;
		}
	} else {
		_rot_speed -= deceletion_speed;
		if ( _rot_speed < 0 ) {
			_rot_speed = 0;
		}
	}
}

void Roomba::holdCrystal( StagePtr stage ) {
	if ( _crystal ) {
		if ( _crystal->isDropDown( ) ||
			 _crystal->isFinished( ) ||
			_state == MOVE_STATE_REFLECTION ||
			_state == MOVE_STATE_REFLECTION_RESTORE ) {
			_crystal->setDropDown( true );
			_crystal = CrystalPtr( );
			return;
		}
		Vector crystal_pos = _crystal->getPos( );
		Vector pos[ 2 ] = {
			_balls[ 0 ]->getPos( ) + _balls[ 0 ]->getVec( ),
			_balls[ 1 ]->getPos( ) + _balls[ 1 ]->getVec( )
		};
		pos[ 0 ].z = START_POS[ 0 ].z;
		pos[ 1 ].z = START_POS[ 1 ].z;
		crystal_pos.z = START_POS[ 0 ].z;
		Vector line = pos[ 1 ] - pos[ 0 ];
		pos[ 0 ] += line.normalize( ) * BALL_RADIUS * 3;
		pos[ 1 ] -= line.normalize( ) * BALL_RADIUS * 3;
		line = pos[ 1 ] - pos[ 0 ];
		Vector to_crystal = crystal_pos - pos[ 0 ];
		double angle = line.angle( to_crystal );
		double cos_length = to_crystal.getLength( ) * cos( angle );
		Vector vec;
		if ( cos_length < 0 ) {							//線より外側
			vec = pos[ 0 ] - crystal_pos;
		} else if ( cos_length > line.getLength( ) ) {	//線より外側
			vec = pos[ 1 ] - crystal_pos;
		} else {									//線に垂直
			double sin_length = to_crystal.getLength( ) * fabs( sin( angle ) );
			Matrix rot = Matrix::makeTransformRotation( line.cross( to_crystal ), PI / 2 );
			vec = rot.multiply( line ).normalize( ) * sin_length;
			if ( vec.getLength( ) < 0.1 ) {
				vec = vec * ( vec.getLength( ) * 10 );
			}
		}
		Vector to_central = line.normalize( ) * ( line.getLength( ) / 2 - cos_length ) * 0.1;
		vec += to_central;
		_crystal->setVec( vec );
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
	if ( _state != MOVE_STATE_REFLECTION &&
		 _state != MOVE_STATE_REFLECTION_RESTORE ) {
		setVecScale( Vector( ), Vector( ) );
		double scale = ( _balls[ 1 ]->getPos( ) - _balls[ 0 ]->getPos( ) ).getLength( );

		return;
	}

	Vector laser = _balls[ 1 ]->getPos( ) - _balls[ 0 ]->getPos( );
	laser.z = 0;
	Vector vec = laser.normalize( ) * RESTORE_SPEED;
	double scale = ( _balls[ 1 ]->getPos( ) - _balls[ 0 ]->getPos( ) ).getLength( );
	for ( int i = 0; i < 2; i++ ) {
		if ( scale > SCALE_SIZE ) {
				_vec_scale[ i ] += laser.normalize( ) * ( scale - SCALE_SIZE + 0.1 ) * ( 1 - ( i != 0 ) * 2 ) * 0.001;
		} else  if ( scale < MIN_SCALE ) {
				_vec_scale[ i ] += laser.normalize( ) * ( MIN_SCALE - scale + 0.1 ) * ( 1 - ( i != 0 ) * 2 ) * -0.001;
		} else {
			_vec_scale[ i ] = Vector( );
		}
		if ( _state == MOVE_STATE_REFLECTION_RESTORE ) {
			_vec_reflection[ i ].z = _balls[ i ]->getPos( ).z - START_POS[ i ].z;
		}
	}

	if ( _state != MOVE_STATE_REFLECTION ) {
		return;
	}
	for ( int i = 0; i < 2; i++ ) {
		Vector pos = _balls[ i ]->getPos( );
		double vec_z = _vec_reflection[ i ].z;
		if ( _vec_reflection[ i ].getLength( ) > EMERGENCY_DECELERATION_SPEED ) {
			_vec_reflection[ i ] -= _vec_reflection[ i ].normalize( ) * EMERGENCY_DECELERATION_SPEED;
			_vec_reflection[ i ].z = vec_z;
		} else {
			_vec_reflection[ i ] = Vector( );
		}
		if ( pos.z > START_POS[ i ].z ) {
			_vec_reflection[ i ].z -= GRAVITY;
			if ( pos.z + _vec_reflection[ i ].z < START_POS[ i ].z ) {
				if ( fabs( _vec_reflection[ i ].z ) > GRAVITY * 2 ) {
					_vec_reflection[ i ].z *= -0.3;
				} else {
					_vec_reflection[ i ].z = START_POS[ i ].z - pos.z;
				}
			}
		}
		if ( pos.z == START_POS[ i ].z ) {
			if ( _vec_reflection[ i ].z < 0 ) {
				_vec_reflection[ i ].z = 0;
			}
		}
	}
}

void Roomba::moveLiftUp( ) {
	if ( _state != MOVE_STATE_LIFT_UP ) {
		return;
	}
	for ( int i = 0; i < 2; i++ ) {
		Vector ball = _balls[ i ]->getPos( ) + Vector( 0, 0, DELIVERY_FOOT );
		Vector distance = _delivery[ i ].pos - ball;
		if ( distance.z > -0.0001 ) {
			_delivery[ i ].pos -= distance.normalize( ) * 0.1;
			_vec_lift[ i ] = Vector( );
		} else {
			_delivery[ i ].pos.z = ball.z;
			_vec_lift[ i ] = Vector( 0, 0, 0.2 );
		}
	}
}

void Roomba::moveLiftDown( ) {
	if ( _state != MOVE_STATE_LIFT_DOWN ) {
		return;
	}
	for ( int i = 0; i < 2; i++ ) {
		Vector pos = _balls[ i ]->getPos( );
		_vec_lift[ i ].z = -0.2;
		if ( pos.z < START_POS[ i ].z ) {
			_vec_lift[ i ] = ( START_POS[ i ] - pos ).normalize( ) * 0.2;
			_link_break = false;
		}
	}
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

void Roomba::drawEffect( MOVE_STATE state ) {
	if ( _state == MOVE_STATE_REFLECTION ) {
		Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_REPLAY, getCentralPos( ), 0.8, EFFECT_ROTATE ) );
		return;
	}
	if ( state == MOVE_STATE_REFLECTION_RESTORE ||
		 state == MOVE_STATE_NEUTRAL ||
		 state == MOVE_STATE_LIFT_UP ||
		 state == MOVE_STATE_LIFT_DOWN ) {
		return;
	}
	if ( ( _state == MOVE_STATE_ROTATION_RIGHT || _state == MOVE_STATE_ROTATION_LEFT ) &&
		 ( state == MOVE_STATE_ROTATION_RIGHT || state == MOVE_STATE_ROTATION_LEFT ) ) {
		return;
	}

	for ( int i = 0; i < 2; i++ ) {
		Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_CHANGE_ROOMBA_STATE, _balls[ i ]->getPos( ), 0.2, EFFECT_ROTATE ) );
	}
}

Vector Roomba::getDir( ) const {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector roomba_dir = mat.multiply( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) );
	return roomba_dir.normalize( );
}

double Roomba::getRotSpeed( ) const {
	return _rot_speed;
}

double Roomba::getLink( ) const {
	return _link_gauge;
}