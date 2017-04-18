#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"
#include "Ball.h"
#include "Camera.h"
#include "Crystal.h"
#include "Timer.h"
#include "Device.h"

static const Vector START_POS[ 2 ] {
	Vector( 2, 6 ) * WORLD_SCALE + Vector( 0, 0, BALL_RADIUS * 2 ),
	Vector( 2, 6 ) * WORLD_SCALE + Vector( WORLD_SCALE * 2, 0, BALL_RADIUS * 2 ),
};
static const double CENTRIPETAL_POWER = 0.020;
static const double CENTRIPETAL_MIN = 3.5;
static const int KEY_WAIT_TIME = 4;

Roomba::Roomba( ) :
_neutral_count( 0 ),
_state( MOVE_STATE_NEUTRAL ) {
	_balls[ BALL_LEFT  ] = BallPtr( new Ball( START_POS[ 0 ] ) );
	_balls[ BALL_RIGHT ] = BallPtr( new Ball( START_POS[ 1 ] ) );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera, TimerPtr timer ) {
	updateState( );
	move( stage, camera );
	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->update( stage );
	}

	//攻撃
	attack( stage, timer );
}

void Roomba::move( StagePtr stage, CameraPtr camera ) {
	Vector camera_dir = camera->getDir( );
	camera_dir.z = 0;

	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->move( camera_dir, _state, _balls[ i % 2 == 0 ] );
		if ( stage->isCollisionWall( _balls[ i ]->getPos( ) + _balls[ i ]->getVec( ) + _balls[ i ]->getVec( ).normalize( ) * BALL_RADIUS ) ) {
			_balls[ i ]->setAccel( Vector( ) );
			continue;
		}
		Vector vec = getCentralPos( ) - _balls[ i ]->getPos( );
		if ( vec.getLength( ) < CENTRIPETAL_MIN ) {
			continue;
		}
		vec -= vec.normalize( ) * CENTRIPETAL_MIN;
		vec *= CENTRIPETAL_POWER;
		_balls[ i ]->setAccel( _balls[ i ]->getVec( ) + vec );
	}
}

void Roomba::updateState( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	DevicePtr device = Device::getTask( );
	int dir_lx = device->getDirX( );
	int dir_ly = device->getDirY( );
	int dir_rx = device->getRightDirX( );
	int dir_ry = device->getRightDirY( );
	if ( keyboard->isHoldKey( "ARROW_UP"   ) ||
		 keyboard->isHoldKey( "ARROW_DOWN" ) ||
		 keyboard->isHoldKey( "W" ) ||
		 keyboard->isHoldKey( "S" ) ) {
		_state = MOVE_STATE_ROTETION_SIDE;
	}
	if ( ( keyboard->isHoldKey( "ARROW_UP"   ) && keyboard->isHoldKey( "S" ) ) ||
		 ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "W" ) ) ) {
		_state = MOVE_STATE_ROTETION_BOTH;
	}
	if ( ( dir_rx < 0 && dir_lx < 0 ) ||
		 ( dir_rx > 0 && dir_lx > 0 ) ||
		 ( dir_ry < 0 && dir_ly < 0 ) ||
		 ( dir_ry > 0 && dir_ly > 0 ) ) {
		_state = MOVE_STATE_TRANSLATION;
	}
}

void Roomba::attack( StagePtr stage, TimerPtr timer ) {
	bool attacking = ( _balls[ BALL_LEFT ]->isAttacking( ) || _balls[ BALL_RIGHT ]->isAttacking( ) );

	if ( !attacking ) {
		return;
	}
	CrystalPtr crystal =  stage->getHittingCrystal( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
	if ( crystal ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 0, 0, "あたってるよー" );
		crystal->damage( );
		timer->addTime( );
	}
}

void Roomba::draw( ) const {
	bool attacking = ( _balls[ BALL_LEFT ]->isAttacking( ) || _balls[ BALL_RIGHT ]->isAttacking( ) );

	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->draw( );
	}

	if ( attacking ) {
		drawer->drawLine( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
	}
}

Vector Roomba::getCentralPos( ) const {
	Vector pos[ MAX_BALL ];
	for ( int i = 0; i < MAX_BALL; i++ ) {
		pos[ i ] = _balls[ i ]->getPos( );
	}
	Vector central_pos = ( pos[ BALL_LEFT ] + pos[ BALL_RIGHT ] ) * 0.5;

	return central_pos;
}

void Roomba::reset( ) {	
	_neutral_count = 0;
	_state = MOVE_STATE::MOVE_STATE_NEUTRAL;
	_balls[ BALL_LEFT ]->reset( START_POS[ 0 ] );
	_balls[ BALL_RIGHT ]->reset( START_POS[ 1 ]  );
}