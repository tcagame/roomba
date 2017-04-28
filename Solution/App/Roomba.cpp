#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Ball.h"
#include "Camera.h"
#include "Crystal.h"
#include "Timer.h"
#include "Device.h"

static const int ON_NUTRAL_TIME = 3;
static const double ACCEL = 0.18;
static const double ATTACK_START_SPEED = 0.09;
static const double CENTRIPETAL_POWER = 0.03;
static const double CENTRIPETAL_MIN = 7;

static const Vector START_POS[ 2 ] {
	Vector( 3, 6 ) * WORLD_SCALE + Vector( 0, 0, 0 ),
	Vector( 3, 6 ) * WORLD_SCALE + Vector( WORLD_SCALE * 4, 0, 0 )
};

Roomba::Roomba( ) :
_state( MOVE_STATE_TRANSLATION ) {
	_balls[ BALL_LEFT ] = BallPtr( new Ball( START_POS[ 0 ] ) );
	_balls[ BALL_RIGHT ] = BallPtr( new Ball( START_POS[ 1 ] ) );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera, TimerPtr timer ) {
	updateState( camera );
	focus( );
	move( camera );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->deceleration( ACCEL );
		_balls[ i ]->update( stage );
	}

	//攻撃
	attack( stage, timer );
	adjustCameraDir( camera );
}

void Roomba::move( CameraPtr camera ) {	
	Vector camera_dir = camera->getDir( );
	camera_dir.z = 0;
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	switch ( _state ) {
	case MOVE_STATE_NEUTRAL:
		break;
	case MOVE_STATE_TRANSLATION:
		moveTranslation( camera_dir, right_stick, left_stick );
		break;
	case MOVE_STATE_ROTATION_SIDE:
		moveRotationSide( camera, camera_dir, right_stick, left_stick );
		break;
	case MOVE_STATE_ROTATION_BOTH:
		moveRotationBoth( camera_dir, right_stick, left_stick );
		break;
	}
}

void Roomba::focus( ) {
// ボールの加速度に求心力を加算
	for ( int i = 0; i < 2; i++ ) {
		Vector pos0 = _balls[ i ]->getPos( ) + _balls[ i ]->getVec( );
		Vector pos1 = _balls[ ( i == 0 ) ]->getPos( );
		Vector distance = pos1 - pos0;
		Vector vec = distance - distance.normalize( ) * CENTRIPETAL_MIN;
		if ( distance.getLength( ) < CENTRIPETAL_MIN ) {
			_balls[ i ]->addForce( vec );
			return;
		}
		vec *= CENTRIPETAL_POWER;
		_balls[ i ]->addForce( vec );
	}
}

void Roomba::updateState( CameraPtr camera ) {
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	
	MOVE_STATE state = MOVE_STATE_NEUTRAL;
	if ( right_stick.x > 0 ||
		 right_stick.x <  0 ||
		 left_stick.x > 0 ||
		 left_stick.x < 0 || 
		 right_stick.y > 0 ||
		 right_stick.y < 0 ||
		 left_stick.y > 0 ||
		 left_stick.y < 0 ) {
		state = MOVE_STATE_ROTATION_SIDE;
	}
	if ( ( right_stick.x > 0  && left_stick.x < 0 ) ||
		 ( right_stick.x < 0  && left_stick.x > 0 ) ||
		 ( right_stick.y > 0  && left_stick.y < 0 ) ||
		 ( right_stick.y < 0  && left_stick.y > 0 ) ) {
		state = MOVE_STATE_ROTATION_BOTH;
	}
	if ( ( right_stick.x < 0 && left_stick.x < 0 ) ||
		 ( right_stick.x > 0 && left_stick.x > 0 ) ||
		 ( right_stick.y < 0 && left_stick.y < 0 ) ||
		 ( right_stick.y > 0 && left_stick.y > 0 ) ) {
		state = MOVE_STATE_TRANSLATION;
	}
	if ( state == MOVE_STATE_NEUTRAL ) {
		_neutral++;
	} else {
		_neutral = 0;
	}
	if ( state != _state ) {
		if ( state == MOVE_STATE_NEUTRAL &&
			 _neutral < ON_NUTRAL_TIME ) {
			state = _state;
		}
		_state = state;
		//checkLeftRight( camera );
	}
}

void Roomba::attack( StagePtr stage, TimerPtr timer ) {
	if ( _balls[ BALL_LEFT ]->getVec( ).getLength( ) > ATTACK_START_SPEED ||
		 _balls[ BALL_RIGHT ]->getVec( ).getLength( ) > ATTACK_START_SPEED ) {
		CrystalPtr crystal =  stage->getHittingCrystal( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
		if ( crystal ) {
			DrawerPtr drawer = Drawer::getTask( );
			drawer->drawString( 0, 0, "あたってるよー" );
			crystal->damage( );
			timer->addTime( );
		}
	}
}

void Roomba::moveTranslation( const Vector& camera_dir, const Vector& right, const Vector& left ) {
	Matrix mat = Matrix::makeTransformRotation( camera_dir.cross( Vector( 0, -1 ) ), camera_dir.angle( Vector( 0, -1 ) ) );

	Vector dir_left = left;
	Vector dir_right = right;

	dir_left = mat.multiply( dir_left );
	dir_right = mat.multiply( dir_right );

	Vector vec_left = dir_left.normalize( ) * ACCEL;
	Vector vec_right = dir_right.normalize( ) * ACCEL;

	_balls[ BALL_LEFT ]->addForce( vec_left );
	_balls[ BALL_RIGHT ]->addForce( vec_right );
}

void Roomba::moveRotationBoth( const Vector& camera_dir, Vector right, Vector left ) {	
	if ( fabs( right.x ) > 70 ) {
		right.y = right.x;
	}
	if ( fabs( left.x ) > 70 ) {
		left.y = left.x;
	}
	right.x = 0;
	left.x = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector ball_left = _balls[ BALL_LEFT ]->getPos( ) + _balls[ BALL_LEFT ]->getVec( );
	Vector ball_right = _balls[ BALL_RIGHT ]->getPos( ) + _balls[ BALL_RIGHT ]->getVec( );
	Vector roomba_dir = mat.multiply( ball_left - ball_right );
	mat = Matrix::makeTransformRotation( roomba_dir.cross( Vector( 0, -1 ) ), roomba_dir.angle( Vector( 0, -1 ) ) );

	Vector dir_left = left * -1;
	Vector dir_right = right * -1;

	dir_left = mat.multiply( dir_left );
	dir_right = mat.multiply( dir_right );

	Vector vec_left = dir_left.normalize( ) * ACCEL;
	Vector vec_right = dir_right.normalize( ) * ACCEL;

	_balls[ BALL_LEFT ]->addForce( vec_left );
	_balls[ BALL_RIGHT ]->addForce( vec_right );
}

void Roomba::moveRotationSide( CameraPtr camera, const Vector& camera_dir, Vector right, Vector left ) {
	if ( fabs( right.x ) > 70 ) {
		right.y = right.x;
	}
	if ( fabs( left.x ) > 70 ) {
		left.y = left.x;
	}
	right.x = 0;
	left.x = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector roomba_dir = mat.multiply( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) );
	mat = Matrix::makeTransformRotation( roomba_dir.cross( Vector( 0, -1 ) ), roomba_dir.angle( Vector( 0, -1 ) ) );
	
	Vector dir_left = left * -1;
	Vector dir_right = right * -1;

	dir_left = mat.multiply( dir_left );
	dir_right = mat.multiply( dir_right );

	Vector vec_left = dir_left.normalize( ) * ACCEL;
	Vector vec_right = dir_right.normalize( ) * ACCEL;

	_balls[ BALL_LEFT ]->addForce( vec_left );
	_balls[ BALL_RIGHT ]->addForce( vec_right );

	if ( dir_left.getLength2( ) < 100 ) {
		_balls[ BALL_LEFT ]->addForce( _balls[ BALL_LEFT ]->getVec( ) * -1 );
	}
	if ( dir_right.getLength2( ) < 100 ) {
		_balls[ BALL_RIGHT ]->addForce( _balls[ BALL_RIGHT ]->getVec( ) * -1 );
	}
	
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->draw( );
	}

	if ( _balls[ BALL_LEFT ]->getVec( ).getLength( ) > ATTACK_START_SPEED ||
		 _balls[ BALL_RIGHT ]->getVec( ).getLength( ) > ATTACK_START_SPEED ) {
		drawer->drawLine( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
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

void Roomba::adjustCameraDir( CameraPtr camera ) {
	if ( _state == MOVE_STATE_NEUTRAL || 
		 _state == MOVE_STATE_ROTATION_SIDE || 
		 _state == MOVE_STATE_TRANSLATION ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), PI / 2 );
		Vector roomba_dir = mat.multiply( _balls[ 0 ]->getPos( ) - _balls[ 1 ]->getPos( ) );
		Vector camera_dir = camera->getDir( );
		camera_dir.z = 0;
		camera_dir = camera_dir.normalize( );
		roomba_dir = roomba_dir.normalize( );
		Vector axis = roomba_dir.cross( camera_dir );
		double angle = roomba_dir.angle( camera_dir );
		if ( angle > 0.01 ) {
			camera->rotation( axis, angle );
		}
	}
}