#include "Ball.h"
#include "define.h"
#include "Keyboard.h"
#include "Stage.h"

static const double ACCEL = 0.05;
static const double MAX_SPEED = 0.5;
static const double ATTACK_START_SPEED = 0.2;
static const double DECELERATION_SPEED = 0.4;

Ball::Ball( Vector pos ) :
_pos( pos ) {
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

void Ball::move( Vector dir, Roomba::MOVE_STATE state, BallPtr target ) {

	bool hold_key[ MAX_KEY ] = { false };

	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( _left ) {
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
	}
	if ( !_left ) {
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
	}
	switch ( state ) {
	case Roomba::MOVE_STATE_TRANSLATION:
		moveTranslation( dir, hold_key );
		break;
	case Roomba::MOVE_STATE_ROTETION_BOTH:
		moveRotetionBoth( target->getPos( ), hold_key, _left );
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

void Ball::moveRotetionBoth( Vector other_pos, bool hold_key[ ], bool left ) {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector dir = mat.multiply( other_pos - _pos );
	if ( left ) {
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
	double accel = ACCEL;
	if ( !_left ) {
		accel *= -1;
	}
	if ( hold_key[ KEY_UP ] ) {
		_vec -= dir.normalize( ) * accel;
		target->setAccel( Vector( ) );
	}
	if ( hold_key[ KEY_DOWN ] ) {
		_vec += dir.normalize( ) * accel;
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

void Ball::neutral( Vector dir, Vector other_pos ) {
	_vec -= _vec * DECELERATION_SPEED;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector vec = mat.multiply( other_pos - _pos );
	double dot = dir.dot( vec );
	if ( dot < 0 ) {
		_left = true;
	}
	if ( dot > 0 ) {
		_left = false;
	}
}

bool Ball::isAttacking( ) const {
	return _vec.getLength( ) > ATTACK_START_SPEED;
}