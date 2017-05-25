#include "Viewer.h"

static const int VIEW_RANGE = 8;
static const Vector OFFSET[ 5 ] = {
	Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),
	Vector( STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),
	Vector( 0, -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
	Vector( 0, STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
	Vector( )
};

Viewer::Viewer( ) :
_map_num_x( 1 ),
_map_num_y( 1 ) {
}


Viewer::~Viewer( ) {
}

void Viewer::update( Vector roomba_pos ) {
	_map_num_x = roomba_pos.x / STAGE_WIDTH_NUM / WORLD_SCALE;
	_map_num_y = roomba_pos.y / STAGE_HEIGHT_NUM / WORLD_SCALE;
}

void Viewer::drawModelMDL( Drawer::ModelMDL mdl ) const {
	DrawerPtr drawer = Drawer::getTask( );
	while ( mdl.pos.x > STAGE_WIDTH_NUM * WORLD_SCALE ) {
		mdl.pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( mdl.pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE ) {
		mdl.pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	char flag = getInViewFlag( mdl.pos );
	mdl.pos += Vector( _map_num_x * STAGE_WIDTH_NUM * WORLD_SCALE, _map_num_y * STAGE_HEIGHT_NUM * WORLD_SCALE );
	drawer->setModelMDL( mdl );
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

void Viewer::drawModelMDLTransfer( Drawer::ModelMDL mdl ) const {
	DrawerPtr drawer = Drawer::getTask( );
	mdl.pos += Vector( _map_num_x * STAGE_WIDTH_NUM * WORLD_SCALE, _map_num_y * STAGE_HEIGHT_NUM * WORLD_SCALE );
	drawer->setModelMDL( mdl );
	const int OFF_X[ 8 ] = { 4, 4, 0, 0, 0, 1, 1, 1 };
	const int OFF_Y[ 8 ] = { 2, 3, 2, 4, 3, 2, 4, 3 };
	for ( int i = 0; i < 8; i++ ) {
		Drawer::ModelMDL model = mdl;
		model.pos += OFFSET[ OFF_X[ i ] ] + OFFSET[ OFF_Y[ i ] ];
		drawer->setModelMDL( model );
	}
}

char Viewer::getInViewFlag( Vector pos ) const {
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