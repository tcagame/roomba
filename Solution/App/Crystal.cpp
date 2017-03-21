#include "Crystal.h"
#include "Drawer.h"
#include "define.h"


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