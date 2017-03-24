#include "Ball.h"
#include "define.h"
#include "Keyboard.h"
#include "Stage.h"

static const double ACCEL = 0.1;

Ball::Ball( Vector pos, Roomba::BALL type ) :
_pos( pos ),
_type( type ) {
}


Ball::~Ball( ) {
}

void Ball::update( StagePtr stage ) {
	if ( !stage->isCollisionWall( _pos + _vec + Vector( 1, 1, 0 ) ) ) {
		_pos += _vec;
	}
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

void Ball::addAccel( Vector vec ) {
	_vec += vec;
}

void Ball::move( Vector dir, Roomba::MOVE_STATE state ) {
	deceleration( );
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
	case Roomba::MOVE_STATE_TRANSLATION:
		moveTranslation( dir, hold_key );
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

void Ball::moveRotetionBoth( Vector dir, bool hold_key[ ] ) {

}

void Ball::moveRotetionSide( Vector dir, bool hold_key[ ] ) {

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