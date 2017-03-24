#include "Ball.h"
#include "define.h"
#include "Keyboard.h"


static const double ACCEL = 0.1;

Ball::Ball( Vector pos ) :
_pos( pos ) {
}


Ball::~Ball( ) {
}

void Ball::update( ) {
	move( );
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

void Ball::move( ) {
	deceleration( );
	_pos += _vec;
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