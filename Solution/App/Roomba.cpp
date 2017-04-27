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
	_balls[ 0 ] = BallPtr( new Ball( START_POS[ 0 ] ) );
	_balls[ 1 ] = BallPtr( new Ball( START_POS[ 1 ] ) );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera, TimerPtr timer ) {
	updateState( camera );

	for ( int i = 0; i < 2; i++ ) {
		move( i, camera );
		_balls[ i ]->update( stage );
	}

	//UŒ‚
	attack( stage, timer );
}

void Roomba::move( int i, CameraPtr camera ) {	
	Vector camera_dir = camera->getDir( );
	camera_dir.z = 0;
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	switch ( _state ) {
	case MOVE_STATE_NEUTRAL:
		_balls[ i ]->deceleration( ACCEL );
		focus( i );
		break;
	case MOVE_STATE_TRANSLATION:
		moveTranslation( i, camera_dir, right_stick, left_stick );
		focus( i );
		break;
	case MOVE_STATE_ROTATION_SIDE:
		focus( i ); // Ž²‚Ì”»’è‚ªmoveRotetionBoth‚É‚ ‚é‚½‚ßæ‚É‹S—Í‚ð—^‚¦‚é
		moveRotetionSide( i, camera_dir, right_stick, left_stick );
		break;
	case MOVE_STATE_ROTATION_BOTH:
		moveRotetionBoth( i, camera_dir, right_stick, left_stick );
		focus( i );
		break;
	}
}

void Roomba::focus( int i ) {
// ƒ{[ƒ‹‚Ì‰Á‘¬“x‚É‹S—Í‚ð‰ÁŽZ
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
		checkLeftRight( camera );
	}
}

void Roomba::attack( StagePtr stage, TimerPtr timer ) {
	if ( _balls[ 0 ]->getVec( ).getLength( ) > ATTACK_START_SPEED ||
		 _balls[ 1 ]->getVec( ).getLength( ) > ATTACK_START_SPEED ) {
		CrystalPtr crystal =  stage->getHittingCrystal( _balls[ 0 ]->getPos( ), _balls[ 1 ]->getPos( ) );
		if ( crystal ) {
			DrawerPtr drawer = Drawer::getTask( );
			drawer->drawString( 0, 0, "‚ ‚½‚Á‚Ä‚é‚æ[" );
			crystal->damage( );
			timer->addTime( );
		}
	}
}

void Roomba::moveTranslation( int i, const Vector& camera_dir, const Vector& right, const Vector& left ) {
	Matrix mat = Matrix::makeTransformRotation( camera_dir.cross( Vector( 0, -1 ) ), camera_dir.angle( Vector( 0, -1 ) ) );
	Vector dir = left;
	if ( i == 1 ) {
		dir = right;
	}
	dir = mat.multiply( dir );
	Vector vec = dir.normalize( ) * ACCEL;
	_balls[ i ]->addForce( vec );
}

void Roomba::moveRotetionBoth( int i, const Vector& camera_dir, Vector right, Vector left ) {	
	if ( fabs( right.x ) > 70 ) {
		right.y = right.x;
	}
	if ( fabs( left.x ) > 70 ) {
		left.y = left.x;
	}
	right.x = 0;
	left.x = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector ball0 = _balls[ 0 ]->getPos( ) + _balls[ 0 ]->getVec( );
	Vector ball1 = _balls[ 1 ]->getPos( ) + _balls[ 1 ]->getVec( );
	Vector roomba_dir = mat.multiply( ball0 - ball1 );
	mat = Matrix::makeTransformRotation( roomba_dir.cross( Vector( 0, -1 ) ), roomba_dir.angle( Vector( 0, -1 ) ) );
	Vector dir = left * -1;
	if ( i == 1 ) {
		dir = right * -1;
	}
	dir = mat.multiply( dir );
	Vector vec = dir.normalize( ) * ACCEL;
	_balls[ i ]->addForce( vec );
}

void Roomba::moveRotetionSide( int i, const Vector& camera_dir, Vector right, Vector left ) {
	if ( fabs( right.x ) > 70 ) {
		right.y = right.x;
	}
	if ( fabs( left.x ) > 70 ) {
		left.y = left.x;
	}
	right.x = 0;
	left.x = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector roomba_dir = mat.multiply( _balls[ 0 ]->getPos( ) - _balls[ 1 ]->getPos( ) );
	mat = Matrix::makeTransformRotation( roomba_dir.cross( Vector( 0, -1 ) ), roomba_dir.angle( Vector( 0, -1 ) ) );
	
	Vector dir = left * -1;
	if ( i == 1 ) {
		dir = right * -1;
	}
	dir = mat.multiply( dir );
	Vector vec = dir.normalize( ) * ACCEL;
	_balls[ i ]->addForce( vec );
	if ( dir.getLength( ) < 10 ) {
		// Ž©g‚ªŽ²‚¾‚Á‚½‚ç‰Á‘¬“x‚ð‚O‚É‚·‚é
		_balls[ i ]->addForce( _balls[ i ]->getVec( ) * -1 );
	}
	
	
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->draw( );
	}

	if ( _balls[ 0 ]->getVec( ).getLength( ) > ATTACK_START_SPEED ||
		 _balls[ 1 ]->getVec( ).getLength( ) > ATTACK_START_SPEED ) {
		drawer->drawLine( _balls[ 0 ]->getPos( ), _balls[ 1 ]->getPos( ) );
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
	_balls[ 0 ]->reset( START_POS[ 0 ] );
	_balls[ 1 ]->reset( START_POS[ 1 ] );
}

void Roomba::checkLeftRight( CameraPtr camera ) {	
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector vec = mat.multiply( _balls[ 0 ]->getPos( ) - _balls[ 1 ]->getPos( ) );
	double dot = camera->getDir( ).dot( vec );
	if ( dot > 0 ) {
		BallPtr tmp = _balls[ 0 ];
		_balls[ 0 ] = _balls[ 1 ];
		_balls[ 1 ] = tmp;
	}
}