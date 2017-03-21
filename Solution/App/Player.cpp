#include "Player.h"
#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"

const double GRAVITY = -0.1;
const double MOVE_SPEED = 0.5;
const double SHRINK_SPEED = 0.4;
static const Vector ROOMBA_SCALE( 2, 2, 2 ); 

Player::Player( ) :
_range( 5 ),
_pos( 4, 0, 10 ),
_dir( 1, 1, 0 ),
_state( STATE::STATE_NEUTRAL ) {
	_roombas[ ROOMBA::ROOMBA_LEFT  ] = RoombaPtr( new Roomba( true  ) );
	_roombas[ ROOMBA::ROOMBA_RIGHT ] = RoombaPtr( new Roomba( false ) );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadMV1Model( MV1::MV1_ROOMBA, "Model/Roomba/source/sphere.mv1" );
}


Player::~Player( ) {
}

void Player::update( StagePtr stage ) {
	fall( );
	if ( isCollision( stage ) ) {
		_vec = Vector( );
	}
	move( );
	_pos += _vec;
	updateRoomba( );
}

void Player::fall( ) {
	_vec.z += GRAVITY;
}

void Player::move( ) {
	switch( _state ){
	case STATE::STATE_NEUTRAL:
		neutral( );
		break;
		//ステート変更
	case STATE::STATE_TRANSLATION:
		translation( );
		break;
	case STATE::STATE_ROTETION:
		rotetion( );
		break;
	}
}

void Player::neutral( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) && keyboard->isHoldKey( "W" ) ) {
		_state = STATE_TRANSLATION;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "S" ) ) {
		_state = STATE_TRANSLATION;
	}
	if ( keyboard->isHoldKey( "ARROW_UP" ) && keyboard->isHoldKey( "S" ) ) {
		_state = STATE_ROTETION;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "W" ) ) {
		_state = STATE_ROTETION;
	}
}

void Player::translation( ) {
	bool hold = false;
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) && keyboard->isHoldKey( "W" ) ) {
		hold = true;
		_vec.y += MOVE_SPEED;
		if ( _range > ROOMBA_SCALE.x ) {
			_range -= SHRINK_SPEED;
		}
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "S" ) ) {
		hold = true;
		_vec.y -= MOVE_SPEED;
		if ( _range > ROOMBA_SCALE.x ) {
			_range -= SHRINK_SPEED;
		}
	}
	if ( !hold ) {
		_state = STATE_NEUTRAL;
	}
}

void Player::rotetion( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {	
	}
	if ( keyboard->isHoldKey( "S" ) ) {
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
	}
	if ( keyboard->isHoldKey( "W" ) ) {
	}
}

void Player::updateRoomba( ) {
	for ( int i = 0; i < MAX_ROOMBA; i++ ) {
		if ( !_roombas[ i ] ) {
			continue;
		}
		double range = _range;
		Vector pos = Vector( );
		if ( _roombas[ i ]->isLeft( ) ) {
			range *= -1;
		}
		pos = _pos + _dir.normalize( ) * range;
		_roombas[ i ]->update( pos, _dir );
	}

}

void Player::draw( ) const {
	for ( int i = 0; i < MAX_ROOMBA; i++ ) {
		if ( !_roombas[ i ] ) {
			continue;
		}
		_roombas[ i ]->draw( );
	}
	//drawLine
	DrawerPtr drawer = Drawer::getTask( );
	Vector pos0 = _roombas[ ROOMBA::ROOMBA_LEFT  ]->getPos( );
	Vector pos1 = _roombas[ ROOMBA::ROOMBA_RIGHT ]->getPos( );
	drawer->drawLine( pos0, pos1 );
}


bool Player::isCollision( StagePtr stage ) {
	bool result = false;
	for ( int i = 0; i < MAX_ROOMBA; i++ ) {
		Vector pos = _roombas[ i ]->getPos( );
		if ( stage->isCollision( _pos, _vec, ROOMBA_SCALE ) ) {
			result = true;
		}
	}
	return result;
}