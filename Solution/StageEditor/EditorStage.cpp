#include "EditorStage.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Application.h"
#include "Binary.h"
#include "define.h"
#include "Camera.h"

static const int CURSOR_WAIT_TIME = 4;
static const double CURSOR_MOVE_SPEED = 0.4;
static const double DELIVERY_POS_Z = EARTH_POS_Z + WORLD_SCALE;


EditorStage::EditorStage( CameraPtr camera ) :
_count( 0 ),
_camera( camera ) {
	reset( );
	MousePtr mouse = Mouse::getTask( );
	_before_mouse_pos = mouse->getPos( );
}


EditorStage::~EditorStage( ) {
}

void EditorStage::update( ) {
	updateCursor( );
	updateMode( );
	edit( );
	debug( );
}

void EditorStage::saveFile( ) const {
	DATA data = getData( );
	BinaryPtr binary = BinaryPtr( new Binary );
	binary->ensure( sizeof( data ) );
	binary->write( (void*)&data, sizeof( data ) );
	ApplicationPtr app = Application::getInstance( );
	std::string filename = "../Resource/Map/" + app->inputString( 0, 40 );
	if ( filename.find( ".stage" ) == std::string::npos ) {
		filename += ".stage";
	}
	app->saveBinary( filename, binary );
}

void EditorStage::loadFile( ) {
	ApplicationPtr app = Application::getInstance( );
	std::string filename = "../Resource/Map/" + app->inputString( 0, 40 );
	BinaryPtr binary( new Binary );
	if ( filename.find( ".stage" ) == std::string::npos ) {
		filename += ".stage";
	}
	if ( !app->loadBinary( filename, binary ) ) {
		return;
	}
	setData( *(DATA*)binary->getPtr( ) );
}

void EditorStage::updateCursor( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	Vector camera_dir = _camera->getDir( ).normalize( );
	camera_dir.z = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( camera_dir ) * -1, Vector( 0, -1 ).angle( camera_dir ) );
	Vector mouse_pos = Mouse::getTask( )->getPos( );
	Vector mouse_vec = mouse_pos - _before_mouse_pos;
	if ( keyboard->isPushKey( "Q" ) ) {
		_cursor_pos = Vector( (int)_cursor_pos.x, (int)_cursor_pos.y );
	}
	if ( mouse_vec.getLength( ) < 0.01 ) {
		return;
	}
	_before_mouse_pos = mouse_pos;
	mouse_vec.x *= 1;
	mouse_vec.y *= 1;
	mouse_vec = mat.multiply( mouse_vec ).normalize( ) * CURSOR_MOVE_SPEED;
	if ( keyboard->isHoldKey( "Q" ) ) {
		double ratio = fabs( mouse_vec.x ) / fabs( mouse_vec.y );
		mouse_vec.y *= ratio;
		mouse_vec = mouse_vec.normalize( ) * CURSOR_MOVE_SPEED;
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
}


void EditorStage::draw( ) const {
	drawModel( );
	drawEarth( );
	drawWall( );
	DrawerPtr drawer = Drawer::getTask( );
	Vector cursor_pos( (int)_cursor_pos.x * WORLD_SCALE + WORLD_SCALE / 2, (int)_cursor_pos.y * WORLD_SCALE  + WORLD_SCALE / 4 );
	drawer->setModelMDL( Drawer::ModelMDL( cursor_pos, MDL_CURSOR ) );
	drawer->drawString( 0, 60, "PHASE:%d", getPhase( ) );
	drawer->drawString( 0, 0, "壁編集:Z　クリスタル編集:X　ステーション編集:C　ロード:F1　セーブ:F2　フェーズ変更:テンキー" );


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
	DATA data = getData( );
	int phase = getPhase( );
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.crystal[ phase ][ i ] != 0 ) {
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
	if ( keyboard->isPushKey( "F1" ) ) {
		drawer->drawString( 0, 20, "ロード" );
		loadFile( );
		reset( );
	}
	if ( keyboard->isPushKey( "F2" ) ) {
		drawer->drawString( 0, 20, "セーブ" );
		saveFile( );
	}
	for ( int i = 0; i < 10; i++ ) {
		std::string key = "NUM" + std::to_string( i );
		if ( keyboard->isPushKey( key ) ) {
			setPhase( i );
		}
	}

	switch ( _mode ) {
	case EDIT_MODE_WALL:
		editWall( );
		break;
	case EDIT_MODE_CRYSTAL:
		editCrystal( );
		break;
	case EDIT_MODE_STATION:
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
		DATA data = getData( );
		if ( data.wall[ idx ] == 0 ) {
			data.wall[ idx ] = 1;
			setData( data );
			loadWall( );
		}
	}
	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		DATA data = getData( );
		if ( data.wall[ idx ] == 1 ) {
			data.wall[ idx ] = 0;
			setData( data );
			loadWall( );
		}
	}
}

void EditorStage::editCrystal( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:クリスタル配置" );
	
	int phase = getPhase( );
	DATA data = getData( );
	int count = 0;
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.crystal[ phase ][ i ] > 0 ) {
			data.crystal[ phase ][ i ] = 1;//修正用
			count++;
		}
	}

	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) && count < MAX_LINK ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.crystal[ phase ][ idx ] == 0 ) {
			data.crystal[ phase ][ idx ] = 1;
			setData( data );
		}
	}
	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.crystal[ phase ][ idx ] != 0 ) {
			data.crystal[ phase ][ idx ] = 0;
			setData( data );
		}
	}
}

void EditorStage::editDelivery( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:ステーション配置" );
	int phase = getPhase( );
	DATA data = getData( );
	bool placed[ MAX_LINK ] = { };

	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.delivery[ phase ][ i ] ) {
			placed[ data.delivery[ phase ][ i ] - 1 ] = true;
		}
	}
	int number = 0;
	for ( int i = 0; i < MAX_LINK; i++ ) {
		if ( !placed[ i ] ) {
			number = i + 1;
			break;
		}
	}

	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) ) && number != 0 ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.delivery[ phase ][ idx ] == 0 ) {
			data.delivery[ phase ][ idx ] = number;
			setData( data );
		}
	}

	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = (int)_cursor_pos.y * STAGE_WIDTH_NUM + (int)_cursor_pos.x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.delivery[ phase ][ idx ] != 0 ) {
			data.delivery[ phase ][ idx ] = 0;
			setData( data );
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
		_mode = EDIT_MODE_STATION;
	}
}

void EditorStage::drawEarth( ) const {
	Vector adjust_pos = Vector( WORLD_SCALE * 2, WORLD_SCALE * 2 + WORLD_SCALE / 3, EARTH_POS_Z );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setModelMDL( Drawer::ModelMDL( adjust_pos, MDL_EARTH ) );
}

void EditorStage::drawWall( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	std::vector< Drawer::ModelMDL > walls = getWalls( );
	std::vector< Drawer::ModelMDL >::const_iterator ite = walls.begin( );
	while ( ite != walls.end( ) ) {
		drawer->setModelMDL( (*ite) );
		ite++;
	}
}

void EditorStage::drawDelivery( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	DATA data = getData( );
	int phase = getPhase( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.delivery[ phase ][ i ] != 0 ) {
			double x = double( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 3;
			double y = double( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2;
			drawer->setModelMDL( Drawer::ModelMDL( Vector( x, y, DELIVERY_POS_Z ), MDL_DELIVERY ) );
		}
	}
}