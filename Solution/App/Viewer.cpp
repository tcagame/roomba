#include "Viewer.h"

Viewer::Viewer( ) {
}


Viewer::~Viewer( ) {
}

void Viewer::update( Vector roomba_pos ) {
	_base_pos = roomba_pos;
}

void Viewer::drawModelMDL( Drawer::ModelMDL mdl ) const {
	DrawerPtr drawer = Drawer::getTask( );
	setViewPos( mdl.pos );
	drawer->setModelMDL( mdl );
}

void Viewer::drawModelMDLMulti( Drawer::ModelMDL mdl ) const {
	DrawerPtr drawer = Drawer::getTask( );
	int x = (int)( _base_pos.x / WORLD_SCALE ) / STAGE_WIDTH_NUM;
	int y = (int)( _base_pos.y / WORLD_SCALE ) / STAGE_HEIGHT_NUM;
	mdl.pos += Vector( x * STAGE_WIDTH_NUM * WORLD_SCALE, y * STAGE_HEIGHT_NUM * WORLD_SCALE );
	drawer->setModelMDL( mdl );
	Vector add1;
	Vector add2;
	if ( (int)( _base_pos.x / WORLD_SCALE ) % STAGE_WIDTH_NUM < STAGE_WIDTH_NUM / 2 ) {
		add1 = Vector(  STAGE_WIDTH_NUM * WORLD_SCALE, 0 );
	} else {
		add1 = Vector( -STAGE_WIDTH_NUM * WORLD_SCALE, 0 );
	}
	if ( (int)( _base_pos.x / WORLD_SCALE ) % STAGE_WIDTH_NUM < STAGE_WIDTH_NUM / 2 ) {
		add2 = Vector( 0,  STAGE_HEIGHT_NUM * WORLD_SCALE );
	} else {
		add2 = Vector( 0, -STAGE_HEIGHT_NUM * WORLD_SCALE );
	}
	//Vector pos = mdl.pos;
	//mdl.pos = pos + add1;
	//drawer->setModelMDL( mdl );
	//mdl.pos = pos + add2;
	//drawer->setModelMDL( mdl );
	//mdl.pos = pos + add1 + add2;
	//drawer->setModelMDL( mdl );
}

void Viewer::drawModelMV1( Stage::MV1_INFO mv1, Matrix scale_rot ) const {
	DrawerPtr drawer = Drawer::getTask( );
	setViewPos( mv1.pos );
	Drawer::ModelMV1 model( scale_rot.multiply( Matrix::makeTransformTranslation( mv1.pos ) ), mv1.type, 0, mv1.time );
	drawer->setModelMV1( model );
}

void Viewer::setViewPos( Vector& pos ) const {
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
}