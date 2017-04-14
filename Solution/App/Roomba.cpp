#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"
#include "Ball.h"
#include "Camera.h"
#include "Crystal.h"
#include "Timer.h"

static const Vector ROOMBA_SCALE( 2, 2, 2 );
static const Vector START_POS( 9, 25, 1 );
static const double CENTRIPETAL_POWER = 0.020;
static const double CENTRIPETAL_MIN = 3.5;
static const int KEY_WAIT_TIME = 4;

Roomba::Roomba( ) :
_neutral_count( 0 ),
_state( MOVE_STATE_NEUTRAL ) {
	_balls[ BALL_LEFT ] = BallPtr( new Ball( START_POS + Vector( 0, 0, 4 ) ) );
	_balls[ BALL_RIGHT ] = BallPtr( new Ball( START_POS + Vector( 10, 0, 4 ) ) );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera, TimerPtr timer ) {
	updateState( );
	move( stage, camera );
	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->update( stage );
	}

	//UŒ‚
	attack( stage, timer );
}

void Roomba::move( StagePtr stage, CameraPtr camera ) {
	Vector dir = camera->getDir( );
	dir.z = 0;

	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->move( dir, _state, _balls[ i % 2 == 0 ] );
		if ( stage->isCollisionWall( _balls[ i ]->getPos( ) + _balls[ i ]->getVec( ) + ROOMBA_SCALE * 0.5 ) ) {
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
	switch ( _state ) {
	case MOVE_STATE_NEUTRAL:
		if ( _neutral_count < KEY_WAIT_TIME ) {
			if ( keyboard->isHoldKey( "ARROW_UP"    ) ||
				 keyboard->isHoldKey( "ARROW_DOWN"  ) ||
				 keyboard->isHoldKey( "ARROW_LEFT"  ) ||
				 keyboard->isHoldKey( "ARROW_RIGHT" ) ||
				 keyboard->isHoldKey( "W" ) ||
				 keyboard->isHoldKey( "S" ) ||
				 keyboard->isHoldKey( "A" ) ||
				 keyboard->isHoldKey( "D" ) ) {
				_neutral_count++;
			}
			break;
		}
		if ( _neutral_count >= KEY_WAIT_TIME ) {
			if ( keyboard->isHoldKey( "ARROW_UP"   ) ||
				 keyboard->isHoldKey( "ARROW_DOWN" ) ||
				 keyboard->isHoldKey( "W" ) ||
				 keyboard->isHoldKey( "S" ) ) {
				_state = MOVE_STATE_ROTETION_SIDE;
			}
			if ( keyboard->isHoldKey( "ARROW_LEFT"  ) && keyboard->isHoldKey( "A" ) ||
				 keyboard->isHoldKey( "ARROW_RIGHT" ) && keyboard->isHoldKey( "D" ) ||
				 keyboard->isHoldKey( "ARROW_UP"    ) && keyboard->isHoldKey( "W" ) ||
				 keyboard->isHoldKey( "ARROW_DOWN"  ) && keyboard->isHoldKey( "S" ) ) {
				_state = MOVE_STATE_TRANSLATION;
			}
			if ( ( keyboard->isHoldKey( "ARROW_UP"   ) && keyboard->isHoldKey( "S" ) ) ||
				 ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "W" ) ) ) {
				_state = MOVE_STATE_ROTETION_BOTH;
			}
			_neutral_count++;
		}
		break;
	case MOVE_STATE_TRANSLATION:
		if ( !( keyboard->isHoldKey( "ARROW_LEFT"  ) && keyboard->isHoldKey( "A" ) ) &&
			 !( keyboard->isHoldKey( "ARROW_RIGHT" ) && keyboard->isHoldKey( "D" ) ) &&
			 !( keyboard->isHoldKey( "ARROW_UP"    ) && keyboard->isHoldKey( "W" ) ) &&
			 !( keyboard->isHoldKey( "ARROW_DOWN"  ) && keyboard->isHoldKey( "S" ) ) ) {
			_state = MOVE_STATE_NEUTRAL;
			_neutral_count = 0;
		}
		break;
	case MOVE_STATE_ROTETION_SIDE:
		if ( !keyboard->isHoldKey( "ARROW_UP" ) &&
			 !keyboard->isHoldKey( "ARROW_DOWN" ) &&
			 !keyboard->isHoldKey( "W" ) &&
			 !keyboard->isHoldKey( "S" ) ) {
			_state = MOVE_STATE_NEUTRAL;
		}
		if ( ( keyboard->isHoldKey( "ARROW_UP"   ) && keyboard->isHoldKey( "S" ) ) ||
				 ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "W" ) ) ) {
				_state = MOVE_STATE_ROTETION_BOTH;
		}
		break;
	case MOVE_STATE_ROTETION_BOTH:
		if ( !( keyboard->isHoldKey( "ARROW_UP"   ) && keyboard->isHoldKey( "S" ) ) &&
			 !( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "W" ) ) ) {
			_state = MOVE_STATE_NEUTRAL;
		}
		break;
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
		drawer->drawString( 0, 0, "‚ ‚½‚Á‚Ä‚é‚æ[" );
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
	_balls[ BALL_LEFT ] = BallPtr( new Ball( START_POS ) );
	_balls[ BALL_RIGHT ] = BallPtr( new Ball( START_POS + Vector( 10, 0, 0 ) ) );
}