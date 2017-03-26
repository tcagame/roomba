#include "Ball.h"
#include "define.h"
#include "Keyboard.h"
#include "Stage.h"

static const double ACCEL = 0.1;
static const double MAX_SPEED = 3.0;
static const double ATTACK_START_SPEED = 1.0;
static const double DECELERATION_SPEED = 0.2;

Ball::Ball( Vector pos, Roomba::BALL type ) :
_pos( pos ),
_type( type ) {
}


Ball::~Ball( ) {
}

void Ball::update( StagePtr stage ) {
	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
	}
	if ( stage->isCollisionWall( _pos + _vec + Vector( 1, 1, 0 ) ) ) {
		_vec = Vector( );
	}
	_pos += _vec;
}

void Ball::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Matrix mat = Matrix::makeTransformTranslation( _pos );
	Drawer::ModelMV1 model = Drawer::ModelMV1( mat, MV1::MV1_BALL, 0, 0 );
	drawer->setModelMV1( model );
}

Vector Ball::getPos( ) const {
	return _pos;
}

Vector Ball::getVec( ) const {
	return _vec;
}

void Ball::addAccel( Vector vec ) {
	_vec += vec;
}

void Ball::move( Vector dir, Roomba::MOVE_STATE state, BallPtr target ) {
	KeyboardPtr keyboard = Keyboard::getTask( );

	bool hold_key[ MAX_KEY ] = { false };
	switch ( _type ) {
	case Roomba::BALL::BALL_LEFT:
		if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
			hold_key[ KEY_UP ] = true;
		}
		if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
			hold_key[ KEY_DOWN ] = true;
		}
		if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
			hold_key[ KEY_LEFT ] = true;
		}
		if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
			hold_key[ KEY_RIGHT ] = true;
		}
		break;
	case Roomba::BALL::BALL_RIGHT:
		if ( keyboard->isHoldKey( "W" ) ) {
			hold_key[ KEY_UP ] = true;
		}
		if ( keyboard->isHoldKey( "S" ) ) {
			hold_key[ KEY_DOWN ] = true;
		}
		if ( keyboard->isHoldKey( "A" ) ) {
			hold_key[ KEY_LEFT ] = true;
		}
		if ( keyboard->isHoldKey( "D" ) ) {
			hold_key[ KEY_RIGHT ] = true;
		}
		break;
	}
	switch ( state ) {
	case Roomba::MOVE_STATE_NEUTRAL:
		neutral( );
		break;
	case Roomba::MOVE_STATE_TRANSLATION:
		moveTranslation( dir, hold_key );
		break;
	case Roomba::MOVE_STATE_ROTETION_BOTH:
		moveRotetionBoth( target->getPos( ), hold_key );
		break;
	case Roomba::MOVE_STATE_ROTETION_SIDE:
		moveRotetionSide( hold_key, target );
		break;
	}
}

void Ball::moveTranslation( Vector dir, bool hold_key[ ] ) {
	if ( hold_key[ KEY_UP ] ) {
		_vec += dir.normalize( ) * ACCEL;
	}
	if ( hold_key[ KEY_DOWN ] ) {
		_vec -= dir.normalize( ) * ACCEL;
	}

	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	dir = mat.multiply( dir );
	if ( hold_key[ KEY_LEFT ] ) {
		_vec += dir.normalize( ) * ACCEL;
	}
	if ( hold_key[ KEY_RIGHT ] ) {
		_vec -= dir.normalize( ) * ACCEL;
	}
}

void Ball::moveRotetionBoth( Vector other_pos, bool hold_key[ ] ) {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector dir = mat.multiply( other_pos - _pos );
	if ( _type == Roomba::BALL_LEFT ) {
		dir *= -1;
	}
	if ( hold_key[ KEY_UP ] ) {
		_vec += dir.normalize( ) * ACCEL;
	}
	if ( hold_key[ KEY_DOWN ] ) {
		_vec -= dir.normalize( ) * ACCEL;
	}
}

void Ball::moveRotetionSide( bool hold_key[ ], BallPtr target ) {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector dir = mat.multiply( target->getPos( ) - _pos );
	if ( hold_key[ KEY_UP ] ) {
		_vec += dir.normalize( ) * ACCEL;
		target->setAccel( Vector( ) );
	}
	if ( hold_key[ KEY_DOWN ] ) {
		_vec -= dir.normalize( ) * ACCEL;
		target->setAccel( Vector( ) );
	}
}

void Ball::deceleration( ) {
	//Œ¸‘¬
	if ( _vec.x > 0 ) {
		_vec.x -= ACCEL / 4;
		if ( _vec.x < 0 ) {
			_vec.x = 0;
		}
	}
	if ( _vec.y > 0 ) {
		_vec.y -= ACCEL / 4;
		if ( _vec.y < 0 ) {
			_vec.y = 0;
		}
	}
	if ( _vec.x < 0 ) {
		_vec.x += ACCEL / 4;
		if ( _vec.x > 0 ) {
			_vec.x = 0;
		}
	}
	if ( _vec.y < 0 ) {
		_vec.y += ACCEL / 4;
		if ( _vec.y > 0 ) {
			_vec.y = 0;
		}
	}
}

void Ball::setAccel( Vector vec ) {
	_vec = vec;
}

void Ball::neutral( ) {
	_vec -= _vec * DECELERATION_SPEED;
}

bool Ball::isAttacking( ) const {
	return _vec.getLength( ) > ATTACK_START_SPEED;
}