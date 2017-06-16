#include "SceneSelect.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"
#include "Game.h"
#include "Application.h"
#include "Sound.h"

static const int SELECT_SIZE  = 512;
static const int SELECT_CENTER_X = 430 / 2;
static const int SELECT_CENTER_Y = 42 / 2;
static const int NUM_SIZE = 64;
static const int NUM_CENTER = 64 / 2;
static const int PITCH = 150; 
static const int MOVE_SPEED = 1;
static const int MAX_COUNT = 10;
static const int THICK_FRAME_SIZE = 57;
static const int TRIANGLE_CENTER_X = 312 / 2;

SceneSelect::SceneSelect( ) :
_select( 1 ),
_count( 0 ),
_ispush( false ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_CIRCLE, "select/circle.png" );
	drawer->loadGraph( GRAPH_STAGE_SELECT, "select/Stage Select.png" );
	drawer->loadGraph( GRAPH_NUM_1, "select/1.png" );
	drawer->loadGraph( GRAPH_NUM_2, "select/2.png" );
	drawer->loadGraph( GRAPH_NUM_3, "select/3.png" );
	freazeSelect( );
}


SceneSelect::~SceneSelect( ) {
}

Scene::NEXT SceneSelect::update( ) {
	SoundPtr sound = Sound::getTask( );
	DevicePtr device = Device::getTask( );
	Vector right_stick = Vector( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick = Vector( device->getDirX( ), device->getDirY( ) );

	if ( right_stick.y > 0 && left_stick.y < 0 ) {
		sound->playSE( "se_maoudamashii_system45.wav" );
		GamePtr game = Game::getTask( );
		game->setStage( _select );
		return NEXT_STAGE;
	}
	if ( _count == 0 ) {
		freazeSelect( );
		if ( left_stick.x > 0 && !_ispush ) {
			sound->playSE( "se_maoudamashii_system43.wav" );
			_count++;
			_select++;
			_rot_right = true;
			_ispush = true;
		}
		if ( left_stick.x < 0 && !_ispush ) {
			sound->playSE( "se_maoudamashii_system43.wav" );
			_rot_right = false;
			_count++;
			_select++;
			_ispush = true;
		}
	}
	if ( left_stick.x == 0 && _ispush ) {
		_ispush = false;
	}

	if ( _count != 0 ) {
		moveSelect( );
		_count++;
		_count %= MAX_COUNT;
	}


	_select = abs( _select ) % 3;

	draw( );
	return NEXT_CONTINUE;
}

void SceneSelect::draw( ) {
	drawRogo( );
	drawTriangle( );
	drawSelect( );
	drawFrame( );
	
}

void SceneSelect::drawRogo( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - SELECT_CENTER_X, HEIGHT / 5, 46, 86, 430, 50 ), GRAPH_STAGE_SELECT );
	drawer->setSprite( sprite );
	
}

void SceneSelect::drawSelect( ) {
	DrawerPtr drawer = Drawer::getTask( );
	int select = _select;

	if ( select == 0 ) {
		select = 3;
	}


	GRAPH graph = (GRAPH)( GRAPH_NUM_1 + ( select - 1 ) );
	
	// stage
	//ê^ÇÒíÜ
	if ( graph == (GRAPH)( GRAPH_NUM_3 + 1 ) ) {
		graph = GRAPH_NUM_1;
	}
	if ( graph == (GRAPH)( GRAPH_NUM_1 - 1 ) ) {
		graph = GRAPH_NUM_3;
	}
	Drawer::Sprite sprite_1( Drawer::Transform( _pos[ 0 ].x, _pos[ 0 ].y, 0, 0, NUM_SIZE, NUM_SIZE  ), graph );
	drawer->setSprite( sprite_1 );
	//ç∂
	if ( _rot_right ) {
		graph = (GRAPH)( graph + 1 );
	} else {
		graph = (GRAPH)( graph - 1 );
	}

	if ( graph == (GRAPH)( GRAPH_NUM_3 + 1 ) ) {
		graph = GRAPH_NUM_1;
	}
	if ( graph == (GRAPH)( GRAPH_NUM_1 - 1 ) ) {
		graph = GRAPH_NUM_3;
	}
	Drawer::Sprite sprite_2( Drawer::Transform( _pos[ 1 ].x, _pos[ 1 ].y, 0, 0, NUM_SIZE, NUM_SIZE, _pos[ 1 ].x + NUM_SIZE * 3 / 5, _pos[ 1 ].y + NUM_SIZE * 3 / 5 ), graph );
	drawer->setSprite( sprite_2 );
	//âE
	if ( _rot_right ) {
		graph = (GRAPH)( graph + 1 );
	} else {
		graph = (GRAPH)( graph - 1 );
	}

	if ( graph == (GRAPH)( GRAPH_NUM_3 + 1 ) ) {
		graph = GRAPH_NUM_1;
	}
	if ( graph == (GRAPH)( GRAPH_NUM_1 - 1 ) ) {
		graph = GRAPH_NUM_3;
	}
	Drawer::Sprite sprite_3( Drawer::Transform( _pos[ 2 ].x, _pos[ 2 ].y, 0, 0, NUM_SIZE, NUM_SIZE, _pos[ 2 ].x + NUM_SIZE * 3 / 5, _pos[ 2 ].y + NUM_SIZE * 3 / 5 ), graph );
	drawer->setSprite( sprite_3 );
	
}

void SceneSelect::moveSelect( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	const Vector target1( WIDTH / 2 - NUM_CENTER,  HEIGHT / 2 + HEIGHT / 5 );
	const Vector target2( target1.x - ( WIDTH / 2 - NUM_SIZE * 5 ), target1.y - SELECT_CENTER_Y / 4 - HEIGHT / 10 );
	const Vector target3( target1.x + ( WIDTH / 2 - NUM_SIZE * 5 ), target1.y - SELECT_CENTER_Y / 4 - HEIGHT / 10 );
	Vector vec1;
	Vector vec2;
	Vector vec3;
	if ( _rot_right ) {
		vec1 = ( target3 - target1 ) * 0.1;
		vec2 = ( target1 - target2 ) * 0.1;
		vec3= ( target2 - target3 ) * 0.1;
	} else {
		vec1 = ( target2 - target1 ) * 0.1;
		vec2 = ( target3 - target2 ) * 0.1;
		vec3 = ( target1 - target3 ) * 0.1;
	}

	_pos[ 0 ] += vec1;
	_pos[ 1 ] += vec2;
	_pos[ 2 ] += vec3;
}

void SceneSelect::freazeSelect( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	_pos[ 0 ] = Vector( WIDTH / 2 - NUM_CENTER, HEIGHT / 2 + HEIGHT / 5 );
	_pos[ 1 ] = Vector( _pos[ 0 ].x - ( WIDTH / 2 - NUM_SIZE * 5 ), _pos[ 0 ].y - SELECT_CENTER_Y / 4 - HEIGHT / 10 );
	_pos[ 2 ] = Vector( _pos[ 0 ].x + ( WIDTH / 2 - NUM_SIZE * 5 ), _pos[ 0 ].y - SELECT_CENTER_Y / 4 - HEIGHT / 10 );
}

void SceneSelect::drawFrame( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );

	//â©êFÇ¢ëæòg
	const int T_SX[ 4 ] = { 0, WIDTH - THICK_FRAME_SIZE, 0, WIDTH - THICK_FRAME_SIZE };
	const int T_SY[ 4 ] = { 0, 0, HEIGHT - THICK_FRAME_SIZE, HEIGHT - THICK_FRAME_SIZE };
	const int T_TX[ 4 ] = { 4, SELECT_SIZE - THICK_FRAME_SIZE - 6, 6, SELECT_SIZE - THICK_FRAME_SIZE - 9 };
	const int T_TY[ 4 ] = { 4, 4, SELECT_SIZE - THICK_FRAME_SIZE - 4, SELECT_SIZE - THICK_FRAME_SIZE - 4 };

	for ( int i = 0; i < 4; i++ ) {
		Drawer::Transform trans( T_SX[ i ], T_SY[ i ], T_TX[ i ], T_TY[ i ], THICK_FRAME_SIZE, THICK_FRAME_SIZE );
		Drawer::Sprite sprite( trans, GRAPH_STAGE_SELECT );
		drawer->setSprite( sprite );
	}

	//îíÇ¢ç◊òg
	const int SY[ 2 ] = { 11, HEIGHT - 11 };
	const int SY_2[ 2 ] = { 0, HEIGHT };

	const int SX[ 2 ] = { 11, WIDTH - 11 };
	const int SX_2[ 2 ] = { 0, WIDTH - 3 };

	for ( int i = 0; i < 2; i++ ) {
		Drawer::Transform trans( THICK_FRAME_SIZE, SY[ i ], THICK_FRAME_SIZE + 4, 0, SELECT_SIZE - ( THICK_FRAME_SIZE + 11 ) * 2, 11, WIDTH - THICK_FRAME_SIZE, SY_2[ i ] );
		Drawer::Sprite sprite( trans, GRAPH_STAGE_SELECT );
		drawer->setSprite( sprite );
	}
	for ( int i = 0; i < 2; i++ ) {
		Drawer::Transform trans( SX[ i ], THICK_FRAME_SIZE, 0, THICK_FRAME_SIZE + 5, 9, SELECT_SIZE - ( THICK_FRAME_SIZE + 9 ) * 2, SX_2[ i ], HEIGHT - THICK_FRAME_SIZE );
		Drawer::Sprite sprite( trans, GRAPH_STAGE_SELECT );
		drawer->setSprite( sprite );
	}
}

void SceneSelect::drawTriangle( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - TRIANGLE_CENTER_X, HEIGHT / 2 + HEIGHT / 5, 100, 225, 305, 70 ), GRAPH_STAGE_SELECT );
	drawer->setSprite( sprite );
}