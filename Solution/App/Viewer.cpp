#include "Viewer.h"

static const int VIEW_RANGE = 8;
static const Vector OFFSET[ 8 ] = {
		Vector( STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),
		Vector( STAGE_WIDTH_NUM * WORLD_SCALE, STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
		Vector( STAGE_WIDTH_NUM * WORLD_SCALE, -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
		Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),
		Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
		Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
		Vector( 0,  STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
		Vector( 0,  -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
};

Viewer::Viewer( ) {
}


Viewer::~Viewer( ) {
}

void Viewer::drawModelMDL( Drawer::ModelMDL mdl ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setModelMDL( mdl );
	if ( isInViewRange( mdl.pos ) ) {
		for ( int i = 0; i < 8; i++ ) {
			Drawer::ModelMDL model = mdl;
			model.pos += OFFSET[ i ];
			drawer->setModelMDL( mdl );
		}
	}
}

bool Viewer::isInViewRange( Vector pos ) {
	bool result = true;
	if ( pos.x > VIEW_RANGE * WORLD_SCALE &&
		 pos.x < ( STAGE_WIDTH_NUM - VIEW_RANGE ) * WORLD_SCALE &&
		 pos.y > VIEW_RANGE * WORLD_SCALE &&
		 pos.y < ( STAGE_HEIGHT_NUM - VIEW_RANGE ) * WORLD_SCALE ) {
		result = false;
	}
	return result;
}