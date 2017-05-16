#include "EditorStage.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Application.h"
#include "Binary.h"
#include "define.h"

static const int CURSOR_WAIT_TIME = 2;

EditorStage::EditorStage( ) :
_cursor_x( 0 ),
_cursor_y( 0 ),
_count( 0 ) {
	reset( );
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
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		cursor_y++;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		cursor_y--;
	}
	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		cursor_x++;
	}
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
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
}


void EditorStage::draw( ) const {
	drawModel( );
	DrawerPtr drawer = Drawer::getTask( );
	Vector cursor_pos( _cursor_x * WORLD_SCALE + WORLD_SCALE / 2, _cursor_y * WORLD_SCALE  + WORLD_SCALE / 4 );
	drawer->setModelMDL( Drawer::ModelMDL( cursor_pos, MDL_CURSOR ) );
	drawer->drawString( 0, 60, "WAVE:%d", getPhase( ) );
	drawer->drawString( 0, 0, "�ǕҏW:Z�@�N���X�^���ҏW:X�@�X�e�[�V�����ҏW:C�@�Z�[�u:F1�@���[�h:F2�@�t�F�[�Y�ύX:�e���L�[" );


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
			MDL mdl = (MDL)( (int)MDL_CRYSTAL_0 + data.crystal[ phase ][ i ] - 1 );
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
		drawer->drawString( 0, 20, "���[�h" );
		loadFile( );
	}
	if ( keyboard->isPushKey( "F2" ) ) {
		drawer->drawString( 0, 20, "�Z�[�u" );
		saveFile( );
	}
	if ( keyboard->isPushKey( "NUM0" ) ) {
		setPhase( 0 );
	}
	if ( keyboard->isPushKey( "NUM1" ) ) {
		setPhase( 1 );
	}
	if ( keyboard->isPushKey( "NUM2" ) ) {
		setPhase( 2 );
	}
	if ( keyboard->isPushKey( "NUM3" ) ) {
		setPhase( 3 );
	}
	if ( keyboard->isPushKey( "NUM4" ) ) {
		setPhase( 4 );
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
	drawer->drawString( 0, 80, "MODE:�ǔz�u" );
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
	drawer->drawString( 0, 80, "MODE:�N���X�^���z�u" );
	
	int phase = getPhase( );
	DATA data = getData( );
	bool num[ MAX_LINK ];
	for ( int i = 0; i < MAX_LINK; i++ ) {
		num[ i ] = false;
	}

	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.crystal[ phase ][ i ] > 0 ) {
			num[ data.crystal[ phase ][ i ] - 1 ] = true;
		}
	}
	int number = 0;
	for ( int i = 0; i < MAX_LINK; i++ ) {
		if ( !num[ i ] ) {
			number = i + 1;
			break;
		}
	}

	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) && number != 0 ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( data.crystal[ phase ][ idx ] == 0 ) {
			data.crystal[ phase ][ idx ] = number;
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
	drawer->drawString( 0, 80, "MODE:�X�e�[�V�����z�u" );
	int phase = getPhase( );
	DATA data = getData( );
	bool num[ MAX_LINK ];
	for ( int i = 0; i < MAX_LINK; i++ ) {
		num[ i ] = false;
	}

	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.station[ phase ][ i ] ) {
			num[ data.station[ phase ][ i ] - 1 ] = true;
		}
	}
	int number = 0;
	for ( int i = 0; i < MAX_LINK; i++ ) {
		if ( !num[ i ] ) {
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