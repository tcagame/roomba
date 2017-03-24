#include "Crystal.h"
#include "Drawer.h"
#include "define.h"
#include "mathmatics.h"

static const double CRYSTAL_RADIUS = 2.0;

Crystal::Crystal( Vector pos ) :
_pos( pos ) {

}


Crystal::~Crystal( ) {

}

void Crystal::draw( ) {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMDL model( _pos, MDL::MDL_CRYSTAL );
	drawer->setModelMDL( model );
}

bool Crystal::isHitting( Vector pos0, Vector pos1 ) {
	//pos0‚Æpos1‚ÌŠÔ‚ÉƒNƒŠƒXƒ^ƒ‹‚ª‚ ‚é‚©‚Ç‚¤‚©
	bool hitting = false;
	Vector vec0 = _pos - pos0;
	Vector vec1 = pos1 - pos0;
	double angle = vec0.angle( vec1 );
	if ( fabs( angle ) > PI / 2 ) {
		return false;
	}
	if ( vec0.getLength( ) > vec1.getLength( ) ) {
		return false;
	}
	double distance = vec0.getLength( ) * fabs( sin( angle ) );
	if ( fabs( distance ) < CRYSTAL_RADIUS ) {
		hitting = true;
	}
	return hitting;
}
