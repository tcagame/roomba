#include "EditorStage.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Application.h"
#include "Binary.h"
#include "Camera.h"
#include "Model.h"

static const double CURSOR_MOVE_SPEED = 0.5;
static const double DELIVERY_POS_Z = FLOOR_POS_Z;
std::string FILENAME[ 32 ] = {
	"" ,
	"0_1.mdl" ,
	"0_2.mdl" ,
	"0_3.mdl" ,
	"0_4.mdl" ,
	"0_5.mdl" ,
	"" ,
	"" ,
	"0_8.mdl" ,
	"" ,
	"0_10.mdl",
	"",
	"0_12.mdl",
	"",
	"",
	"0_15.mdl",
	"1_0.mdl" ,
	"1_1.mdl" ,
	"1_2.mdl" ,
	"1_3.mdl" ,
	"1_4.mdl" ,
	"1_5.mdl" ,
	"" ,
	"" ,
	"1_8.mdl" ,
	"" ,
	"1_10.mdl",
	"",
	"1_12.mdl",
	"",
	"",
	"1_15.mdl",
};


EditorStage::EditorStage( CameraPtr camera ) :
_count( 0 ),
_camera( camera ),
_save( false ) {
	reset( );
	MousePtr mouse = Mouse::getTask( );
	_before_mouse_pos = mouse->getPos( );
	Matrix scale = Matrix::makeTransformScaling( FLOOR_SIZE );

	int width = ( STAGE_WIDTH_NUM / FLOOR_CHIP_SIZE );
	int height = ( STAGE_HEIGHT_NUM / FLOOR_CHIP_SIZE );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		Vector pos = Vector( i % STAGE_WIDTH_NUM * WORLD_SCALE, i / STAGE_WIDTH_NUM * WORLD_SCALE );
		_floor[ i ] = Drawer::ModelMDL( pos, MDL_FLOOR );
	}
	_delivery = ModelPtr( new Model );
	_delivery->load( "../Resource/Model/Delivery/delivery.mdl" );
	_delivery->multiply( Matrix::makeTransformScaling( DELIVERY_SIZE ) );
	load( 0 );
	checkPlacedNum( );
}


EditorStage::~EditorStage( ) {
}

void EditorStage::update( CameraPtr camera ) {
	updateCursor( );
	updateMode( );
	edit( );
	debug( );
}

void EditorStage::saveFile( ) const {
	int stage_num = getStageNum( );
	std::string filename = "../Resource/Map/" + std::to_string( stage_num ) + ".stage";
	saveData( filename );
	saveWall( );
	saveFloor( );
}

void EditorStage::saveWall( ) const {
	ModelPtr base_model[ 32 ];
	for ( int i = 0; i < 32; i++ ) {
		if ( FILENAME[ i ] != "" ) {
			base_model[ i ] = ModelPtr( new Model );
			base_model[ i ]->load( "../Resource/Model/Stage/" + FILENAME[ i ] );
		}
	}

	std::vector< Drawer::ModelMDL >::const_iterator ite = _walls.begin( );
	int size = _walls.size( );
	
	int count = 0;
	for ( int i = 0; i < WALL_DIV_SIZE; i++ ) {
		ModelPtr wall_div_model( new Model );
		while ( ite != _walls.end( ) ) {
			int idx = ( count * WALL_DIV_SIZE ) / size;
			if ( idx != i ) {
				if ( idx == i + 1 ) {
					break;
				}
			}
			int type = (*ite).type - MDL_WALL_0_0;
			Vector pos = Vector( (*ite).pos.x, (*ite).pos.y ) * ( 1 / WALL_SIZE.x );
			base_model[ type ]->setPos( pos );
			wall_div_model->mergeModel( base_model[ type ] );
			count++;
			ite++;
		}
		int stage_num = getStageNum( );
		wall_div_model->save( "../Resource/Model/Stage/_wall_" + std::to_string( stage_num ) + "_" + std::to_string( i ) + ".mdl" );
		wall_div_model.reset( );
	}
}
void EditorStage::saveFloor( ) const {
	ModelPtr floor_model = ModelPtr( new Model );
	ModelPtr tmp = ModelPtr( new Model );
	tmp->load( "../Resource/Model/Stage/floor.mdl" );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		tmp->setPos( _floor[ i ].pos * ( 1 / FLOOR_SIZE.x ) );
		floor_model->mergeModel( tmp );
	}
	floor_model->save( "../Resource/Model/Stage/_floor" );
}

void EditorStage::updateCursor( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	Vector camera_dir = _camera->getDir( ).normalize( );
	camera_dir.z = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( camera_dir ) * -1, Vector( 0, -1 ).angle( camera_dir ) );
	Vector mouse_pos = Mouse::getTask( )->getPos( );
	Vector mouse_vec = mouse_pos - _before_mouse_pos;
	_before_mouse_pos = mouse_pos;
	mouse_vec.x *= 1;
	mouse_vec.y *= 1;
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		mouse_vec.y -= CURSOR_MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		mouse_vec.y += CURSOR_MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		mouse_vec.x -= CURSOR_MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
		mouse_vec.x += CURSOR_MOVE_SPEED;
	}
	if ( mouse_vec.getLength( ) < 0.01 ) {
		return;
	}
	mouse_vec = mat.multiply( mouse_vec ).normalize( ) * CURSOR_MOVE_SPEED;
	if ( keyboard->isPushKey( "Q" ) ) {
		_cursor_pos.x = (int)_cursor_pos.x;
		_cursor_pos.y = (int)_cursor_pos.y;
	}
	if ( keyboard->isHoldKey( "Q" ) ) {
		mouse_vec.x = ( fabs( mouse_vec.x ) > 0.001 ) * ( 1 - ( mouse_vec.x < 0 ) * 2 ) * CURSOR_MOVE_SPEED;
		mouse_vec.y = ( fabs( mouse_vec.y ) > 0.001 ) * ( 1 - ( mouse_vec.y < 0 ) * 2 ) * CURSOR_MOVE_SPEED;
	}
	_cursor_pos += mouse_vec;


	if ( _cursor_pos.x < 0 ) {
		_cursor_pos.x = 0;
	}
	if ( _cursor_pos.y < 0 ) {
		_cursor_pos.y = 0;
	}
	if ( _cursor_pos.x > STAGE_WIDTH_NUM ) {
		_cursor_pos.x = STAGE_WIDTH_NUM - 0.1;
	}
	if ( _cursor_pos.y > STAGE_HEIGHT_NUM  ) {
		_cursor_pos.y = STAGE_HEIGHT_NUM - 0.1;
	}
	//z
	int x = (int)_cursor_pos.x;
	int y = (int)_cursor_pos.y;
	int idx = x + y * STAGE_WIDTH_NUM;
	Stage::DATA data = getData( idx );
	_cursor_pos.z = 0;
	if ( data.delivery == 1 ) {
		_cursor_pos.z = DELIVERY_POS_Z + DELIVERY_SIZE.z;
	}
	if ( data.crystal == 1 ) {
		_cursor_pos.z = CRYSTAL_SIZE.z;
	}
	if ( data.wall == 1 ) {
		_cursor_pos.z = WALL_SIZE.z * 2;
	}
	_cursor_pos.z += 1;
}


void EditorStage::draw( ) const {
	drawModel( );
	drawFloor( );
	drawWall( );
	DrawerPtr drawer = Drawer::getTask( );
	Vector cursor_pos( (int)_cursor_pos.x * WORLD_SCALE + WORLD_SCALE / 2, (int)_cursor_pos.y * WORLD_SCALE  + WORLD_SCALE / 4, _cursor_pos.z );
	drawer->setModelMDL( Drawer::ModelMDL( cursor_pos, MDL_CURSOR ) );
	drawer->drawString( 0, 0, "壁編集:Z　クリスタル編集:X　デリバー編集:C　セーブ:F2　カメラ移動:テンキー" );


	for ( int i = 0; i <= STAGE_WIDTH_NUM; i++ ) {
		double x = STAGE_MODEL_SIZE / WORLD_SCALE * (double)i;
		Vector pos0( x, 0 );
		Vector pos1( x, STAGE_MODEL_SIZE / WORLD_SCALE * STAGE_HEIGHT_NUM );
		drawer->drawLine( pos0, pos1 );
	}
	for ( int i = 0; i <= STAGE_HEIGHT_NUM; i++ ) {
		double y = STAGE_MODEL_SIZE / WORLD_SCALE * (double)i;
		Vector pos0( 0, y );
		Vector pos1( STAGE_MODEL_SIZE / WORLD_SCALE * STAGE_WIDTH_NUM, y );
		drawer->drawLine( pos0, pos1 );
	}

}

void EditorStage::drawCrystal( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( getData( i ).crystal != 0 ) {
			MDL mdl = (MDL)( MDL_CRYSTAL );
			double x = double( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2;
			double y = double( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 3;
			Drawer::ModelMDL model( Vector( x, y, 0 ), mdl );
			drawer->setModelMDL( model );
		}
	}
}

void EditorStage::edit( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	DrawerPtr drawer = Drawer::getTask( );
	if ( _save ) {
		for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
			if ( _deliverys[ i ] ) {
				_deliverys[ i ].reset( );
				_deliverys[ i ] = ModelPtr( );
			}
		}

		saveFile( );
		loadDelivery( );
		_save = false;
	}
	if ( keyboard->isPushKey( "F2" ) ) {
		_save = true;
		drawer->drawString( 0, 20, "セーブ中..." );
	}

	switch ( _mode ) {
	case EDIT_MODE_WALL:
		editWall( );
		break;
	case EDIT_MODE_CRYSTAL:
		editCrystal( );
		break;
	case EDIT_MODE_DELIVERY:
		editDelivery( );
		break;
	}
}

void EditorStage::editWall( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:壁配置" );
	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		DATA data = getData( idx );
		if ( data.wall == 0 ) {
			data.wall = 1;
			setData( data, idx );
			loadWall( );
		}
	}
	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		DATA data = getData( idx );
		if ( data.wall == 1 ) {
			data.wall = 0;
			setData( data, idx );
			loadWall( );
		}
	}
}

void EditorStage::editCrystal( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:クリスタル配置" );
	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );

	if ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) && _placed_crystal_num < MAX_LINK ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		DATA data = getData( idx );
		if ( data.crystal == 0 ) {
			data.crystal = 1;
			_placed_crystal_num++;
			setData( data, idx );
		}
	}
	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		DATA data = getData( idx );
		if ( data.crystal != 0 ) {
			_placed_crystal_num--;
			data.crystal = 0;
			setData( data, idx );
		}
	}
}

void EditorStage::editDelivery( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:デリバー配置" );
	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) ) && _placed_delivery_num < MAX_LINK ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		DATA data = getData( idx );
		if ( data.delivery == 0 ) {
			data.delivery = 1;
			_placed_delivery_num++;
			setData( data, idx );


			//モデル
			if ( _deliverys[ idx ] ) {
				_deliverys[ idx ]->reset( );
			} else {
				_deliverys[ idx ] = ModelPtr( new Model );
			}
			double x = double( idx % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 3;
			double y = double( idx / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2;
			_deliverys[ idx ] = ModelPtr( new Model );
			_deliverys[ idx ]->mergeModel( _delivery );
			_deliverys[ idx ]->setPos( Vector( x, y, DELIVERY_POS_Z ) );
		}
	}

	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		DATA data = getData( idx );
		if ( data.delivery != 0 ) {
			data.delivery = 0;
			_placed_delivery_num--;
			setData( data, idx );

			//モデル
			if ( _deliverys[ idx ] ) {
				_deliverys[ idx ].reset( );
				_deliverys[ idx ] = ModelPtr( );
			}
		}
	}
}

void EditorStage::updateMode( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isPushKey( "Z" ) ) {
		_mode = EDIT_MODE_WALL;
	}
	if ( keyboard->isPushKey( "X" ) ) {
		_mode = EDIT_MODE_CRYSTAL;
	}
	if ( keyboard->isPushKey( "C" ) ) {
		_mode = EDIT_MODE_DELIVERY;
	}
}

void EditorStage::drawFloor( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < ( STAGE_WIDTH_NUM / FLOOR_CHIP_SIZE ) * ( STAGE_HEIGHT_NUM / FLOOR_CHIP_SIZE ); i++ ) {
		drawer->setModelMDL( _floor[ i ] );
	}
}

void EditorStage::drawWall( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	std::vector< Drawer::ModelMDL >::const_iterator ite = _walls.begin( );
	while ( ite != _walls.end( ) ) {
		Drawer::ModelMDL mdl = (*ite);
		drawer->setModelMDL( mdl );
		ite++;
	}
}

void EditorStage::drawDelivery( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	std::array< Stage::DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > data = getData( );
			Matrix rot = Matrix::makeTransformRotation( Vector( 1, 0, 0 ), PI / 2 );
			Matrix scale = Matrix::makeTransformScaling( DELIVERY_SIZE );
			Matrix scale_rot = scale.multiply( rot );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data[ i ].delivery != 0 ) {
			Drawer::ModelSelf self;
			self.graph = GRAPH_MDL_DELIVERY;
			self.add = false;
			self.model = _deliverys[ i ];
			drawer->setModelSelf( self );
		}
	}
}

void EditorStage::loadWall( ) {
	_walls.clear( );
	const int OFFSET_X[ 8 ] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	const int OFFSET_Y[ 8 ] = { -1, -1, 1, 1, -1, 1, 0, 0 };

	for ( int i = 0; i < 16; i++ ) {
		int tmp = 1;
		for ( int j = 0; j < 4; j++ ) {
			tmp *= 2;
			int type = 0;
			if ( i % tmp >= tmp / 2 ) {
				type = 1;
			}
		}
	}
	std::array< Stage::DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > data = getData( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		//壁生成
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		Vector pos( x * WORLD_SCALE + WORLD_SCALE / 2, y * WORLD_SCALE + WORLD_SCALE / 2, 0 );
		int type = data[ i ].wall;
		if ( type != 0 && type != 1 ) {
			continue;
		}
		unsigned char flag = 0;
		for ( int j = 0; j < 4; j++ ) {
			int idx0 = i + OFFSET_X[ j ];
			int idx1 = i + OFFSET_Y[ j ] * STAGE_WIDTH_NUM;
			if ( x + OFFSET_X[ j ] < 0 ) {
				idx0 += STAGE_WIDTH_NUM;
			}
			if ( x + OFFSET_X[ j ] >= STAGE_WIDTH_NUM ) {
				idx0 -= STAGE_WIDTH_NUM;
			}
			if ( y + OFFSET_Y[ j ] < 0 ) {
				idx1 += STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM;
			}
			if ( y + OFFSET_Y[ j ] >= STAGE_HEIGHT_NUM ) {
				idx1 -= STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM;
			}
			if ( type == 1 ) {
				if ( data[ idx0 ].wall == 0 && data[ idx1 ].wall == 0 ) {
					flag |= 1 << j;
				}
			}
			if ( type == 0 ) {
				if ( data[ idx0 ].wall == 1 && data[ idx1 ].wall == 1 ) {
					flag |= 1 << j;
				}
			}
		}
		if ( type == 0 && flag == 0 ) {
				continue;
		}
		MDL wall_type = (MDL)( MDL_WALL_0_0 + type * 16 + flag );
		if ( wall_type == MDL_WALL_0_0 ) {
			continue;
		}
		if ( ( wall_type > MDL_WALL_0_0 && wall_type <= MDL_WALL_0_15 ) ||
			wall_type == MDL_WALL_1_0 ) {
		}
		Drawer::ModelMDL mdl;
		mdl.pos = pos;
		mdl.type = wall_type;
		_walls.push_back( mdl );
	}
}

void EditorStage::checkPlacedNum( ) {
	_placed_crystal_num = 0;
	_placed_delivery_num = 0;
	std::array< Stage::DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > data = getData( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data[ i ].crystal != 0 ) {
			_placed_crystal_num++;
		}
		if ( data[ i ].delivery != 0 ) {
			_placed_delivery_num++;
		}
	}
}

void EditorStage::loadDelivery( ) {
	_deliverys = { };
	std::array< Stage::DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > data = getData( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data[ i ].delivery != 0 ) {
			if ( !_deliverys[ i ] ) {
				double x = double( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 3;
				double y = double( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2;
				_deliverys[ i ] = ModelPtr( new Model );
				_deliverys[ i ]->mergeModel( _delivery );
				_deliverys[ i ]->setPos( Vector( x, y, DELIVERY_POS_Z ) );
			}
		}
	}
}

void EditorStage::reset( ) {
	Stage::reset( );
	checkPlacedNum( );
}