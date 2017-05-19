#include "Viewer.h"

static const int VIEW_RANGE = 8;
static const Vector OFFSET[ 4 ] = {
	Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),
	Vector( STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),
	Vector( 0, -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
	Vector( 0, STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
};

Viewer::Viewer( ) {
}


Viewer::~Viewer( ) {
}

void Viewer::drawModelMDL( Drawer::ModelMDL mdl ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setModelMDL( mdl );
	char flag = getInViewFlag( mdl.pos );
	if ( flag != 0 ) {
		char check = 1;
		for ( int i = 0; i < 4; i++ ) {
			if ( flag & check ) {
				Drawer::ModelMDL model = mdl;
				model.pos += OFFSET[ i ];	
				drawer->setModelMDL( model );
			}
			check *= 2;
		}
	}
}

char Viewer::getInViewFlag( Vector pos ) {
	char flag = 0;
	if ( pos.x > VIEW_RANGE * WORLD_SCALE ) {
		flag |= 1;
	}
	if ( pos.x < ( STAGE_WIDTH_NUM - VIEW_RANGE ) * WORLD_SCALE ) {
		flag |= 2;
	}
	if ( pos.y > VIEW_RANGE * WORLD_SCALE ) {
		flag |= 4;
	}
	if ( pos.y < ( STAGE_HEIGHT_NUM - VIEW_RANGE ) * WORLD_SCALE ) {
		flag |= 8;
	}
	return flag;
}