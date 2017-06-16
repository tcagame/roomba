#include "Viewer.h"

static const Vector OFFSET[ 5 ] = {
	Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),
	Vector( STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),
	Vector( 0, -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
	Vector( 0, STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),
	Vector( )
};

Viewer::Viewer( ) {
}


Viewer::~Viewer( ) {
}

void Viewer::update( Vector roomba_pos ) {
	_base_pos = roomba_pos;
}

void Viewer::drawModelMDL( Drawer::ModelMDL mdl ) const {
	DrawerPtr drawer = Drawer::getTask( );
	mdl.pos = getViewPos( mdl.pos );
	drawer->setModelMDL( mdl );
}

void Viewer::drawModelMV1( Stage::MV1_INFO mv1, Vector scale ) const {
	DrawerPtr drawer = Drawer::getTask( );
	mv1.pos = getViewPos( mv1.pos );
	Matrix mat = Matrix::makeTransformScaling( scale );
	Drawer::ModelMV1 model( mat.multiply( Matrix::makeTransformTranslation( mv1.pos ) ), mv1.type, 0 );
	drawer->setModelMV1( model );
}

void Viewer::drawModelMDLTransfer( Drawer::ModelMDL mdl ) const {
	DrawerPtr drawer = Drawer::getTask( );
	int map_x = (int)( _base_pos.x / STAGE_WIDTH_NUM / WORLD_SCALE );
	int map_y = (int)( _base_pos.y / STAGE_HEIGHT_NUM / WORLD_SCALE );
	mdl.pos += Vector( map_x * STAGE_WIDTH_NUM * WORLD_SCALE, map_y * STAGE_HEIGHT_NUM * WORLD_SCALE );
	drawer->setModelMDL( mdl );
	const int OFF_X[ 8 ] = { 4, 4, 0, 0, 0, 1, 1, 1 };
	const int OFF_Y[ 8 ] = { 2, 3, 2, 4, 3, 2, 4, 3 };
	for ( int i = 0; i < 8; i++ ) {
		Drawer::ModelMDL model = mdl;
		model.pos += OFFSET[ OFF_X[ i ] ] + OFFSET[ OFF_Y[ i ] ];
		drawer->setModelMDL( model );
	}
}

Vector Viewer::getViewPos( Vector pos ) const {
	while ( pos.x - _base_pos.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.x - _base_pos.x < -STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.y - _base_pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	while ( pos.y - _base_pos.y < -STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	return pos;
}