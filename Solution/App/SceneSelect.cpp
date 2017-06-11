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

SceneSelect::SceneSelect( ) :
_select( 1 ),
_ispush( false ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_STAGE_SELECT_ROGO, "select/select.png");
	drawer->loadGraph( GRAPH_STAGE_SELECT_1, "select/stage1.png");
	drawer->loadGraph( GRAPH_STAGE_SELECT_2, "select/stage2.png");
	drawer->loadGraph( GRAPH_STAGE_SELECT_3, "select/stage3.png");
	drawer->loadGraph( GRAPH_STAGE_SELECTER, "select/selecter.png");
}


SceneSelect::~SceneSelect( ) {
}

Scene::NEXT SceneSelect::update( ) {
	SoundPtr sound = Sound::getTask( );
	draw( );
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		sound->playSE( "se_maoudamashii_system49.wav" );
		GamePtr game = Game::getTask( );
		game->setStage( _select );
		return NEXT_STAGE;
	}
	if ( device->getDirY( ) > 0 && !_ispush ) {
		sound->playSE( "se_maoudamashii_system43.wav" );
		_select++;
		_ispush = true;
	}
	if ( device->getDirY( ) < 0 && !_ispush ) {
		sound->playSE( "se_maoudamashii_system43.wav" );
		_select--;
		_ispush = true;
	}
	if ( device->getDirY( ) == 0 && _ispush ) {
		sound->playSE( "se_maoudamashii_system43.wav" );
		_ispush = false;
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
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	int x = WIDTH / 4;
	int y = HEIGHT / 3;
	int select = _select;
	if ( _select == 0 ) {
		select = 3;
	}
	// –îˆó
	Drawer::Sprite sprite( Drawer::Transform( x, y + 64 ), GRAPH_STAGE_SELECTER );
	drawer->setSprite( sprite );

	GRAPH graph = (GRAPH)( GRAPH_STAGE_SELECT_1 + ( select - 1 ) );

	// stage
	for ( int i = 0; i < 3; i++, graph = (GRAPH)( graph + 1 ) ) {
		if ( graph == (GRAPH)( GRAPH_STAGE_SELECT_3 + 1 ) ) {
			graph = GRAPH_STAGE_SELECT_1;
		}
		Drawer::Sprite sprite( Drawer::Transform( x + i * PITCH, y + i * PITCH ), graph );
		drawer->setSprite( sprite );
	}
}
