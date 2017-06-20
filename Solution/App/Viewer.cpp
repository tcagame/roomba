#include "Viewer.h"

const Vector OFFSET[ 9 ] = {
	Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ), //ã
	Vector( STAGE_WIDTH_NUM * WORLD_SCALE, 0, 0 ),  //‰º
	Vector( 0, -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),//¶
	Vector( 0, STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ), //‰E
	Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ), //¶ã
	Vector( STAGE_WIDTH_NUM * WORLD_SCALE , STAGE_HEIGHT_NUM * WORLD_SCALE,  0 ),//‰Eã
	Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),//¶‰º
	Vector( STAGE_WIDTH_NUM * WORLD_SCALE , -STAGE_HEIGHT_NUM * WORLD_SCALE, 0 ),//‰E‰º
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

void Viewer::drawModelMDLMulti( Drawer::ModelMDL mdl ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Vector pos = mdl.pos;
	for ( int i = 0; i < 9; i++ ) {
		mdl.pos = pos + OFFSET[ i ];
		drawer->setModelMDL( mdl );
	}
}

void Viewer::drawModelMV1( Stage::MV1_INFO mv1, Matrix scale_rot ) const {
	DrawerPtr drawer = Drawer::getTask( );
	mv1.pos = getViewPos( mv1.pos );
	Drawer::ModelMV1 model( scale_rot.multiply( Matrix::makeTransformTranslation( mv1.pos ) ), mv1.type, 0, mv1.time );
	drawer->setModelMV1( model );
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