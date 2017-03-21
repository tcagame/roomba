#include "Player.h"
#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"

const double GRAVITY = -0.1;
const double SCALING_SPEED = 0.1;
static const Vector ROOMBA_SCALE( 2, 2, 2 );
static const double ACCEL = 0.1;
static const double ROTE_ACCEL = PI / 360;
static const double MAX_ROTE_SPEED = PI / 15;
static const double MAX_SPEED = 0.6;
static const double ROTE_SPEED = 0.1;

Player::Player( ) :
_range( 5 ),
_pos( 4, 0, 10 ),
_dir( 1, 0, 0 ),
_rote_speed( 0 ),
_state( STATE::STATE_NEUTRAL ) {
	_roombas[ ROOMBA::ROOMBA_LEFT  ] = RoombaPtr( new Roomba( true  ) );
	_roombas[ ROOMBA::ROOMBA_RIGHT ] = RoombaPtr( new Roomba( false ) );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadMV1Model( MV1::MV1_ROOMBA, "Model/Roomba/source/sphere.mv1" );
}


Player::~Player( ) {
}

void Player::update( StagePtr stage ) {
	_attack = false;
	fall( );
	move( );
	if ( isCollision( stage ) ) {
		_vec.z = 0;
	}
	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
		_attack = true;
	}
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
	deceleration( );
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
	_range -= SCALING_SPEED;
	if ( _range < ROOMBA_SCALE.x ) {
		_range = ROOMBA_SCALE.x;
	}
	{//回転
		Matrix mat = Matrix( );
		Vector axis = Vector( 0, 0, 1 );
		mat = mat.makeTransformRotation( axis, _rote_speed );
		_dir = mat.multiply( _dir );
	}
}

void Player::translation( ) {
	bool hold = false;
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) && keyboard->isHoldKey( "W" ) ) {
		hold = true;
		_vec.y += ACCEL;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "S" ) ) {
		hold = true;
		_vec.y -= ACCEL;
	}

	_range -= SCALING_SPEED;
	if ( _range < ROOMBA_SCALE.x ) {
		_range = ROOMBA_SCALE.x;
	}
	if ( !hold ) {
		_state = STATE_NEUTRAL;
	}

}

void Player::rotetion( ) {
	bool hold = false;
	
	KeyboardPtr keyboard = Keyboard::getTask( );
	//右ルンバ
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		hold = true;
		_rote_speed -= ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		_rote_speed += ROTE_ACCEL;
		hold = true;
	}
	//左ルンバ
	if ( keyboard->isHoldKey( "S" ) ) {
		hold = true;
		_rote_speed -= ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "W" ) ) {
		_rote_speed += ROTE_ACCEL;
		hold = true;
	}
	if ( _rote_speed > MAX_ROTE_SPEED ) {
		_rote_speed = MAX_ROTE_SPEED;
		_attack = true;
	}
	if ( _rote_speed < -MAX_ROTE_SPEED ) {
		_rote_speed = -MAX_ROTE_SPEED;
		_attack = true;
	}
	_range += SCALING_SPEED;
	if ( !hold ) {
		_state = STATE_NEUTRAL;
	}
	Matrix mat = Matrix( );
	Vector axis = Vector( 0, 0, 1 );
	mat = mat.makeTransformRotation( axis, _rote_speed );
	_dir = mat.multiply( _dir );
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
		{//90度回転させた方向にルンバを配置
			Matrix mat = Matrix( );
			Vector axis = Vector( 0, 0, 1 );
			mat = mat.makeTransformRotation( axis, PI / 2 );
			Vector dir = mat.multiply( _dir );
			pos = _pos + dir.normalize( ) * range;
		}
		_roombas[ i ]->update( pos, _dir );
	}
}

void Player::deceleration( ) {
	//減速
	if ( _vec.x > 0 ) {
		if ( _vec.x < ACCEL ) {
			_vec.x = 0;
		}
		_vec.x -= ACCEL;
	}
	if ( _vec.y > 0 ) {
		if ( _vec.y < ACCEL ) {
			_vec.y = 0;
		}
		_vec.y -= ACCEL;
	}
	if ( _rote_speed > 0 ) {
		_rote_speed -= ROTE_ACCEL / 6;
		if ( _rote_speed < 0 ) {
			_rote_speed = 0;
		}
	}
	if ( _rote_speed< 0 ) {
		_rote_speed += ROTE_ACCEL / 6;
		if ( _rote_speed > 0 ) {
			_rote_speed = 0;
		}
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
	if ( _attack ) {
		DrawerPtr drawer = Drawer::getTask( );
		Vector pos0 = _roombas[ ROOMBA::ROOMBA_LEFT  ]->getPos( );
		Vector pos1 = _roombas[ ROOMBA::ROOMBA_RIGHT ]->getPos( );
		drawer->drawLine( pos0, pos1 );
	}
}

Vector Player::getPos( ) const {
	return _pos;
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