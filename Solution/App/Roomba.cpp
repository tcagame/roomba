#include "Roomba.h"
#include "define.h"


Roomba::Roomba( bool left ) :
_left( left ) {
}


Roomba::~Roomba( ) {
}

void Roomba::update( Vector pos, Vector dir ) {
	_pos = pos;
	_dir = dir;
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Matrix mat = Matrix( );
	mat = mat.makeTransformTranslation( _pos );
	Drawer::ModelMV1 model = Drawer::ModelMV1( mat, MV1::MV1_ROOMBA, 0, 0 );
	drawer->setModelMV1( model );
}

bool Roomba::isLeft( ) const {
	return _left;
}

Vector Roomba::getPos( ) const {
	return _pos;
}
