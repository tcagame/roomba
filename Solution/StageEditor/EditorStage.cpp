#include "EditorStage.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Application.h"
#include "Binary.h"
#include "define.h"

static const int CURSOR_WAIT_TIME = 4;

EditorStage::EditorStage( ) :
_cursor_x( 0 ),
_cursor_y( 0 ),
_count( 0 ) {
	reset( );
	MousePtr mouse = Mouse::getTask( );
	_before_mouse_pos = mouse->getPos( );
	//mouse->changeMouseImage( 0 );
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
	int cursor_x = 0;
	int cursor_y = 0;
	Vector mouse_pos = Mouse::getTask( )->getPos( );
	Vector mouse_vec = mouse_pos - _before_mouse_pos;
	_before_mouse_pos = mouse_pos;
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		cursor_y += 3;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		cursor_y -= 3;
	}
	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		cursor_x += 3;
	}
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
		cursor_x -= 3;
	}
	if ( mouse_vec.y < -0.1 ) {
		cursor_y++;
	}
	if ( mouse_vec.y > 0.1 ) {
		cursor_y--;
	}
	if ( mouse_vec.x < -0.1 ) {
		cursor_x++;
	}
	if ( mouse_vec.x > 0.1 ) {
		cursor_x--;
	}
	if ( cursor_x != 0 || cursor_y != 0 ) {
		_count++;
		if ( _count % CURSOR_WAIT_TIME == 0 ) {
			_cursor_x += cursor_x;
			_cursor_y += cursor_y;
		}
	} else {
		_count = 0;
	}

	if ( _cursor_x < 0 ) {
		_cursor_x = 0;
	}
	if ( _cursor_y < 0 ) {
		_cursor_y = 0;
	}
	if ( _cursor_x >= STAGE_WIDTH_NUM ) {
		_cursor_x = STAGE_WIDTH_NUM - 1;
	}
	if ( _cursor_y >= STAGE_WIDTH_NUM  ) {
		_cursor_y = STAGE_WIDTH_NUM - 1;
	}
}


void EditorStage::draw( ) const {
	drawModel( );
	drawEarth( );
	drawWall( );
	DrawerPtr drawer = Drawer::getTask( );
	Vector cursor_pos( _cursor_x * WORLD_SCALE + WORLD_SCALE / 2, _cursor_y * WORLD_SCALE  + WORLD_SCALE / 4 );
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
		editStation( );
		break;
	}
}

void EditorStage::editWall( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:壁配置" );
	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
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
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
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
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.crystal[ phase ][ idx ] == 0 ) {
			data.crystal[ phase ][ idx ] = 1;
			setData( data );
		}
	}
	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.crystal[ phase ][ idx ] != 0 ) {
			data.crystal[ phase ][ idx ] = 0;
			setData( data );
		}
	}
}

void EditorStage::editStation( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:ステーション配置" );
	int phase = getPhase( );
	DATA data = getData( );
	bool placed[ MAX_LINK ] = { };

	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.station[ phase ][ i ] ) {
			placed[ data.station[ phase ][ i ] - 1 ] = true;
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
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.station[ phase ][ idx ] == 0 ) {
			data.station[ phase ][ idx ] = number;
			setData( data );
		}
	}

	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.station[ phase ][ idx ] != 0 ) {
			data.station[ phase ][ idx ] = 0;
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
	Vector adjust_pos = Vector( WORLD_SCALE * 2, WORLD_SCALE * 2 + WORLD_SCALE / 3, WORLD_SCALE / 6 );
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

void EditorStage::drawStation( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	DATA data = getData( );
	int phase = getPhase( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.station[ phase ][ i ] != 0 ) {
			double x = double( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 3;
			double y = double( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2;
			drawer->setModelMDL( Drawer::ModelMDL( Vector( x, y, 0 ), MDL_STATION ) );
		}
	}
}