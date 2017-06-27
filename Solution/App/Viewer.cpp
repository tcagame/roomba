#include "Viewer.h"
#include "Model.h"
#include "Shadow.h"

const double MARGIN = 0.0001;

Viewer::Viewer( ShadowPtr shadow ) :
_shadow( shadow ) {
}


Viewer::~Viewer( ) {
}

void Viewer::update( Vector roomba_pos ) {
	_old_pos = _base_pos;
	_base_pos = roomba_pos;
}

void Viewer::drawModelMDL( Drawer::ModelMDL mdl ) const {
	DrawerPtr drawer = Drawer::getTask( );
	setViewPos( mdl.pos );
	drawer->setModelMDL( mdl );
}

void Viewer::drawFloor( std::array< ModelPtr, 4 > mdl ) const {
	double div_world_scale = 1 / WORLD_SCALE;
	double div_move = div_world_scale * 2;
	int now_div_x = (int)( _base_pos.x * div_move ) / STAGE_WIDTH_NUM;
	int now_div_y = (int)( _base_pos.y * div_move ) / STAGE_HEIGHT_NUM;
	int old_div_x = (int)( _old_pos.x * div_move ) / STAGE_WIDTH_NUM;
	int old_div_y = (int)( _old_pos.y * div_move ) / STAGE_HEIGHT_NUM;
	int now_x = now_div_x / 2;
	int now_y = now_div_y / 2;
	int move_div_x =  now_div_x - old_div_x;
	int move_div_y =  now_div_y - old_div_y;
	//23
	//01
	if ( move_div_x != 0 ) {
		int move_x = now_x - ( old_div_x / 2 );
		if ( move_x != 0 ) {
			Vector pos = mdl[ 0 ]->getPos( );
			int mdl_x = (int)( ( pos.x + MARGIN ) * div_world_scale ) / STAGE_WIDTH_NUM;
			int diff_x = now_x - mdl_x;
			if ( diff_x != 0 ) {
				ModelPtr tmp = mdl[ 0 ];
				mdl[ 0 ] = mdl[ 1 ];
				mdl[ 1 ] = tmp;
				tmp = mdl[ 2 ];
				mdl[ 2 ] = mdl[ 3 ];
				mdl[ 3 ] = tmp;
			}
		}
		for ( int i = 0; i < 4; i++ ) {
			int check_x = now_x;
			if ( i == 1 || i == 3 ) {
				double x = 0;
				if ( fmod( _base_pos.x * div_world_scale + MARGIN, STAGE_WIDTH_NUM ) > STAGE_WIDTH_NUM * 0.5 ) {
					check_x++;
				} else {
					check_x--;
				}
			}
			Vector pos = mdl[ i ]->getPos( );
			int mdl_x = (int)( ( pos.x + MARGIN ) * div_world_scale ) / STAGE_WIDTH_NUM;
			int diff_x = check_x - mdl_x;
			if ( diff_x != 0 ) {
				pos.x += diff_x * WORLD_SCALE * STAGE_WIDTH_NUM;
				mdl[ i ]->setPos( pos );
			}
		}
	}
	if ( move_div_y != 0 ) {
		int move_y = now_y - ( old_div_y / 2 );
		if ( move_y != 0 ) {
			Vector pos = mdl[ 0 ]->getPos( );
			int mdl_y = (int)( ( pos.y + MARGIN ) * div_world_scale ) / STAGE_HEIGHT_NUM;
			int diff_y = now_y - mdl_y;
			if ( diff_y != 0 ) {
				ModelPtr tmp = mdl[ 0 ];
				mdl[ 0 ] = mdl[ 2 ];
				mdl[ 2 ] = tmp;
				tmp = mdl[ 1 ];
				mdl[ 1 ] = mdl[ 3 ];
				mdl[ 3 ] = tmp;
			}
		}
		for ( int i = 0; i < 4; i++ ) {
			int check_y = now_y;
			if ( i == 2 || i == 3 ) {
				if ( fmod( _base_pos.y * div_world_scale + MARGIN, STAGE_HEIGHT_NUM ) > STAGE_HEIGHT_NUM * 0.5 ) {
					check_y++;
				} else {
					check_y--;
				}
			}
			Vector pos = mdl[ i ]->getPos( );
			int mdl_y = (int)( pos.y * div_world_scale + MARGIN ) / STAGE_HEIGHT_NUM;
			int diff_y = check_y - mdl_y;
			if ( diff_y != 0 ) {
				pos.y += diff_y * WORLD_SCALE * STAGE_HEIGHT_NUM;
				mdl[ i ]->setPos( pos );
			}
		}
	}


	for ( int i = 0; i < 4; i++ ) {
		Drawer::ModelSelf self;
		self.model = mdl[ i ];
		self.graph = GRAPH_FLOOR;
		self.add = false;
		self.z_buffer = true;
		DrawerPtr drawer = Drawer::getTask( );
		drawer->setModelSelf( self );
	}
}

void Viewer::drawWall( std::array< ModelPtr, WALL_DIV_SIZE * 4 > mdl ) const {
	double div_world_scale = 1 / WORLD_SCALE;
	double div_move = div_world_scale * 2;
	int now_div_x = (int)( _base_pos.x * div_move ) / STAGE_WIDTH_NUM;
	int now_div_y = (int)( _base_pos.y * div_move ) / STAGE_HEIGHT_NUM;
	int old_div_x = (int)( _old_pos.x * div_move ) / STAGE_WIDTH_NUM;
	int old_div_y = (int)( _old_pos.y * div_move ) / STAGE_HEIGHT_NUM;
	int now_x = now_div_x / 2;
	int now_y = now_div_y / 2;
	int move_div_x =  now_div_x - old_div_x;
	int move_div_y =  now_div_y - old_div_y;
	//23
	//01
	if ( move_div_x != 0 ) {
		int move_x = now_x - ( old_div_x / 2 );
		if ( move_x != 0 ) {
			Vector pos = mdl[ 0 ]->getPos( );
			int mdl_x = (int)( pos.x * div_world_scale + MARGIN ) / STAGE_WIDTH_NUM;
			int diff_x = now_x - mdl_x;
			if ( diff_x != 0 ) {
				for ( int i = 0; i < WALL_DIV_SIZE; i++ ) {
					int idx0 = WALL_DIV_SIZE * 0 + i;
					int idx1 = WALL_DIV_SIZE * 1 + i;
					int idx2 = WALL_DIV_SIZE * 2 + i;
					int idx3 = WALL_DIV_SIZE * 3 + i;
					ModelPtr tmp = mdl[ idx0 ];
					mdl[ idx0 ] = mdl[ idx1 ];
					mdl[ idx1 ] = tmp;
					tmp = mdl[ idx2 ];
					mdl[ idx2 ] = mdl[ idx3 ];
					mdl[ idx3 ] = tmp;
				}
			}
		}
		for ( int i = 0; i < 4; i++ ) {
			int check_x = now_x;
			if ( i == 1 || i == 3 ) {
				if ( fmod( _base_pos.x * div_world_scale + MARGIN, STAGE_WIDTH_NUM ) > STAGE_WIDTH_NUM * 0.5 ) {
					check_x++;
				} else {
					check_x--;
				}
			}
			for ( int j = 0; j < WALL_DIV_SIZE; j++ ) {
				int idx = i * WALL_DIV_SIZE + j;
				Vector pos = mdl[ idx ]->getPos( );
				int mdl_x = (int)( pos.x * div_world_scale + MARGIN ) / STAGE_WIDTH_NUM;
				int diff_x = check_x - mdl_x;
				if ( diff_x != 0 ) {
					pos.x += diff_x * WORLD_SCALE * STAGE_WIDTH_NUM;
					mdl[ idx ]->setPos( pos );
				}
			}
		}
	}
	if ( move_div_y != 0 ) {
		int move_y = now_y - ( old_div_y / 2 );
		if ( move_y != 0 ) {
			Vector pos = mdl[ 0 ]->getPos( );
			int mdl_y = (int)( ( pos.y + 1 ) * div_world_scale ) / STAGE_HEIGHT_NUM;
			int diff_y = now_y - mdl_y;
			if ( diff_y != 0 ) {
				for ( int i = 0; i < WALL_DIV_SIZE; i++ ) {
					int idx0 = WALL_DIV_SIZE * 0 + i;
					int idx1 = WALL_DIV_SIZE * 1 + i;
					int idx2 = WALL_DIV_SIZE * 2 + i;
					int idx3 = WALL_DIV_SIZE * 3 + i;
					ModelPtr tmp = mdl[ idx0 ];
					mdl[ idx0 ] = mdl[ idx2 ];
					mdl[ idx2 ] = tmp;
					tmp = mdl[ idx1 ];
					mdl[ idx1 ] = mdl[ idx3 ];
					mdl[ idx3 ] = tmp;
				}
			}
		}
		for ( int i = 0; i < 4; i++ ) {
			int check_y = now_y;
			if ( i == 2 || i == 3 ) {
				if ( fmod( _base_pos.y * div_world_scale + MARGIN, STAGE_HEIGHT_NUM ) > STAGE_HEIGHT_NUM * 0.5 ) {
					check_y++;
				} else {
					check_y--;
				}
			}
			for ( int j = 0; j < WALL_DIV_SIZE; j++ ) {
				int idx = i * WALL_DIV_SIZE + j;
				Vector pos = mdl[ idx ]->getPos( );
				int mdl_y = (int)( pos.y * div_world_scale + MARGIN ) / STAGE_HEIGHT_NUM;
				int diff_y = check_y - mdl_y;
				if ( diff_y != 0 ) {
					pos.y += diff_y * WORLD_SCALE * STAGE_HEIGHT_NUM;
					mdl[ idx ]->setPos( pos );
				}
			}
		}
	}

	for ( int i = 0; i < 4 * WALL_DIV_SIZE; i++ ) {
		Drawer::ModelSelf self;
		self.model = mdl[ i ];
		self.graph = GRAPH_WALL;
		self.add = false;
		self.z_buffer = true;
		DrawerPtr drawer = Drawer::getTask( );
		drawer->setModelSelf( self );
	}
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

void Viewer::setShadow( Vector pos, const double scale, bool shift_pos ) {
	if ( shift_pos ) {
		setViewPos( pos );
	}
	_shadow->set( pos, scale );
}