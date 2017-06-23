#include "Viewer.h"
#include "Model.h"

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

void Viewer::drawModelMDLMulti( ModelPtr mdl, GRAPH graph, int idx ) const {
	int x = (int)( _base_pos.x / WORLD_SCALE ) / STAGE_WIDTH_NUM;
	int y = (int)( _base_pos.y / WORLD_SCALE ) / STAGE_HEIGHT_NUM;
	if ( idx == 1 || idx == 3 ) {
		if ( (int)( _base_pos.x / WORLD_SCALE ) % STAGE_WIDTH_NUM > STAGE_WIDTH_NUM / 2 ) {
			x++;
		} else {
			x--;
		}
	}
	if ( idx == 2 || idx == 3 ) {
		if ( (int)( _base_pos.y / WORLD_SCALE ) % STAGE_HEIGHT_NUM > STAGE_HEIGHT_NUM / 2 ) {
			y++;
		} else {
			y--;
		}
	}
	Vector pos = mdl->getPos( );
	int mdl_x = (int)( pos.x / WORLD_SCALE ) / STAGE_WIDTH_NUM;
	int mdl_y = (int)( pos.y / WORLD_SCALE ) / STAGE_HEIGHT_NUM;

	int diff_x = x - mdl_x;
	int diff_y = y - mdl_y;

	if ( diff_x != 0 || diff_y != 0 ) {
		pos.x += diff_x * WORLD_SCALE * STAGE_WIDTH_NUM;
		pos.y += diff_y * WORLD_SCALE * STAGE_HEIGHT_NUM;
		mdl->setPos( pos );
	}
	Drawer::ModelSelf self;
	self.model = mdl;
	self.graph = graph;
	self.add = false;
	self.z_buffer = true;
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setModelSelf( self );
}

void Viewer::drawModelMV1( Stage::MV1_INFO mv1, Matrix scale_rot ) const {
	DrawerPtr drawer = Drawer::getTask( );
	setViewPos( mv1.pos );
	mv1.model.matrix = scale_rot.multiply( Matrix::makeTransformTranslation( mv1.pos ) );
	drawer->setModelMV1( mv1.model );
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