#include "EditorStage.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Application.h"
#include "Binary.h"
#include "define.h"
#include "Camera.h"

static const double CURSOR_MOVE_SPEED = 0.5;
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

void EditorStage::update( CameraPtr camera ) {
	updateCursor( );
	updateMode( );
	edit( );
	debug( );
}

void EditorStage::saveFile( ) const {
	ApplicationPtr app = Application::getInstance( );
	std::string filename = "../Resource/Map/" + app->inputString( 0, 40 );
	saveData( filename );
}

void EditorStage::loadFile( ) {
	ApplicationPtr app = Application::getInstance( );
	std::string filename = "../Resource/Map/" + app->inputString( 0, 40 );
	loadData( filename );
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
}


void EditorStage::draw( ) const {
	drawModel( );
	drawEarth( );
	drawWall( );
	DrawerPtr drawer = Drawer::getTask( );
	Vector cursor_pos( (int)_cursor_pos.x * WORLD_SCALE + WORLD_SCALE / 2, (int)_cursor_pos.y * WORLD_SCALE  + WORLD_SCALE / 4 );
	drawer->setModelMDL( Drawer::ModelMDL( cursor_pos, MDL_CURSOR ) );
	drawer->drawString( 0, 0, "壁編集:Z　クリスタル編集:X　デリバー編集:C　ロード:F1　セーブ:F2　カメラ移動:テンキー" );


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
	if ( keyboard->isPushKey( "F1" ) ) {
		drawer->drawString( 0, 20, "ロード" );
		loadFile( );
		reset( );
	}
	if ( keyboard->isPushKey( "F2" ) ) {
		drawer->drawString( 0, 20, "セーブ" );
		saveFile( );
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
		DATA data = getData( idx );
		if ( data.wall == 0 ) {
			DATA data = DATA( );
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
	
	int count = 0;
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( getData( i ).crystal > 0 ) {
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
		DATA data = getData( idx );
		if ( data.crystal == 0 ) {
			data.crystal = 1;
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
			data.crystal = 0;
			setData( data, idx );
		}
	}
}

void EditorStage::editDelivery( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:デリバー配置" );
	bool placed[ MAX_LINK ] = { };

	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		DATA data = getData( i );
		if ( data.delivery ) {
			placed[ data.delivery - 1 ] = true;
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
		DATA data = getData( idx );
		if ( data.delivery == 0 ) {
			data.delivery = number;
			setData( data, idx );
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
			setData( data, idx );
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
	std::vector< Stage::MV1_INFO > walls = getWalls( );
	std::vector< Stage::MV1_INFO >::const_iterator ite = walls.begin( );
	while ( ite != walls.end( ) ) {
		Stage::MV1_INFO mv1 = (*ite);
		Matrix mat = Matrix::makeTransformScaling( WALL_SIZE );
		Drawer::ModelMV1 model( mat.multiply( Matrix::makeTransformTranslation( mv1.pos ) ), mv1.type, 0 );
		drawer->setModelMV1( model );
		ite++;
	}
}

void EditorStage::drawDelivery( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( getData( i ).delivery != 0 ) {
			double x = double( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 3;
			double y = double( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2;
			Matrix trans = Matrix::makeTransformTranslation( Vector( x, y, 0 ) );
			Matrix rot = Matrix::makeTransformRotation( Vector( 1, 0, 0 ), PI / 2 );
			Matrix scale = Matrix::makeTransformScaling( DELIVERY_SIZE );
			drawer->setModelMV1( Drawer::ModelMV1( scale.multiply( rot ).multiply( trans ), MV1_DELIVERY, 0, 0 ) );
		}
	}
}