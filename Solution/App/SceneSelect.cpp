#include "SceneSelect.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"
#include "Game.h"
#include "Application.h"
#include "Sound.h"

static const int SELECT_WIDTH  = 1024;
static const int SELECT_HEIGHT = 256;
static const int SELECT_CENTER_X = 1450 / 2;
static const int SELECT_CENTER_Y = 180;
static const int PITCH = 150; 
static const int MOVE_SPEED = 3;
static const int MAX_COUNT = 10;

SceneSelect::SceneSelect( ) :
_select( 1 ),
_ispush( false ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_STAGE_SELECT_ROGO, "select/select.png");
	drawer->loadGraph( GRAPH_STAGE_SELECT_1, "select/stage1.png");
	drawer->loadGraph( GRAPH_STAGE_SELECT_2, "select/stage2.png");
	drawer->loadGraph( GRAPH_STAGE_SELECT_3, "select/stage3.png");
	drawer->loadGraph( GRAPH_STAGE_SELECTER, "select/selecter.png");

	freazeSelect( );	
}


SceneSelect::~SceneSelect( ) {
}

Scene::NEXT SceneSelect::update( ) {
	SoundPtr sound = Sound::getTask( );
	draw( );
	DevicePtr device = Device::getTask( );
	Vector right_stick = Vector( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick = Vector( device->getDirX( ), device->getDirY( ) );

	if ( right_stick.y < 0 && left_stick.y < 0 ) {
		sound->playSE( "se_maoudamashii_system45.wav" );
		GamePtr game = Game::getTask( );
		game->setStage( _select );
		return NEXT_STAGE;
	}
	if ( _count == 0 ) {
		freazeSelect( );
		if ( right_stick.y > 0 && left_stick.y < 0 ) {
			sound->playSE( "se_maoudamashii_system43.wav" );
			_count++;
			_select++;
			_ispush = true;
		}
		if ( right_stick.y < 0 && left_stick.y > 0 ) {
			sound->playSE( "se_maoudamashii_system43.wav" );
			_count++;
			_select--;
			_ispush = true;
		}
	}

	if ( _count != 0 ) {
		moveSelect( );
		_count++;
		_count %= MAX_COUNT;
	}

	_select = abs( _select ) % 3;
	return NEXT_CONTINUE;
}

void SceneSelect::draw( ) {
	drawRogo( );
	drawSelect( );
}

void SceneSelect::drawRogo( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - SELECT_WIDTH / 2, HEIGHT / 10 ), GRAPH_STAGE_SELECT_ROGO );
	drawer->setSprite( sprite );
}

void SceneSelect::drawSelect( ) {
	DrawerPtr drawer = Drawer::getTask( );
	int select = _select;
	if ( _select == 0 ) {
		select = 3;
	}
	double tw1 = SELECT_CENTER_X * 1.5;
	double th1 = SELECT_CENTER_Y * 1.5;
	double tw2 = SELECT_CENTER_X / 2;
	double th2 = SELECT_CENTER_Y / 2;

	GRAPH graph = (GRAPH)( GRAPH_STAGE_SELECT_1 );

	// stage
	//^‚ñ’†
	if ( graph > (GRAPH)( GRAPH_STAGE_SELECT_3 + 1 ) ) {
		graph = GRAPH_STAGE_SELECT_1;
	}
	Drawer::Sprite sprite_1( Drawer::Transform( _pos[ 0 ].x, _pos[ 0 ].y, 0, 0, SELECT_WIDTH, SELECT_HEIGHT, _pos[ 0 ].x + tw1, _pos[ 0 ].y + th1 ), graph );
	drawer->setSprite( sprite_1 );
	//¶
	if ( graph > (GRAPH)( GRAPH_STAGE_SELECT_3 + 1 ) ) {
		graph = GRAPH_STAGE_SELECT_1;
	}
	Drawer::Sprite sprite_2( Drawer::Transform( _pos[ 1 ].x, _pos[ 1 ].y, 0, 0, SELECT_WIDTH, SELECT_HEIGHT, _pos[ 1 ].x + tw2, _pos[ 1 ].y + th2 ), graph + 1 );
	drawer->setSprite( sprite_2 );
	//‰E
	if ( graph > (GRAPH)( GRAPH_STAGE_SELECT_3 + 1 ) ) {
		graph = GRAPH_STAGE_SELECT_1;
	}
	Drawer::Sprite sprite_3( Drawer::Transform( _pos[ 2 ].x, _pos[ 2 ].y, 0, 0, SELECT_WIDTH, SELECT_HEIGHT, _pos[ 2 ].x + tw2, _pos[ 2 ].y + th2 ), graph + 2 );
	drawer->setSprite( sprite_3 );
	
}

void SceneSelect::moveSelect( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	const Vector target1( WIDTH / 2 - SELECT_WIDTH / 2, HEIGHT - SELECT_CENTER_Y + 10 );
	const Vector target2( target1.x - SELECT_CENTER_X / 3, target1.y - SELECT_CENTER_Y / 4 );
	const Vector target3( target2.x + SELECT_CENTER_X * 1.2, target2.y - SELECT_CENTER_Y / 4 );

	const Vector vec1 = ( target2 - target1 ) * 0.1;
	const Vector vec2 = ( target3 - target2 ) * 0.1;
	const Vector vec3 = ( target1 - target3 ) * 0.1;

	_pos[ 0 ] += vec1;
	_pos[ 1 ] += vec2;
	_pos[ 2 ] += vec3;
}

void SceneSelect::freazeSelect( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	_pos[ 0 ] = Vector( WIDTH / 2 - SELECT_WIDTH / 2, HEIGHT - SELECT_CENTER_Y + 10 );
	_pos[ 1 ] = Vector( _pos[ 0 ].x - SELECT_CENTER_X / 3, _pos[ 0 ].y - SELECT_CENTER_Y / 4 );
	_pos[ 2 ] = Vector( _pos[ 0 ].x + SELECT_CENTER_X * 1.2, _pos[ 0 ].y - SELECT_CENTER_Y / 4 );
}