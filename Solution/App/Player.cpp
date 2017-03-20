#include "Player.h"
#include "Roomba.h"
#include "define.h"
#include "Stage.h"

const double GRAVITY = -0.1;
static const Vector ROOMBA_SCALE( 1, 1, 1 ); 

Player::Player( ) :
_range( 5 ),
_pos( 4, 0, 10 ),
_dir( 1, 0, 0 ) {
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
	_pos += _vec;
	updateRoomba( );
}

void Player::fall( ) {
	_vec.z += GRAVITY;
}

void Player::updateRoomba( ) {
	for ( int i = 0; i < MAX_ROOMBA; i++ ) {
		if ( !_roombas[ i ] ) {
			continue;
		}
		int range = _range;
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