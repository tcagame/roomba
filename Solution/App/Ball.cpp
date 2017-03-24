#include "Ball.h"
#include "define.h"

Ball::Ball( Vector pos ) :
_pos( pos ) {
}


Ball::~Ball( ) {
}

void Ball::update( Vector pos ) {
	_pos = pos;
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
