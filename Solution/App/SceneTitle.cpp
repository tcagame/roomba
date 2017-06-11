#include "SceneTitle.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"
#include "Application.h"
#include "Sound.h"

static const int TITLE_WIDTH  = 1024;
static const int TITLE_HEIGHT = 256;
static const int DRAW_TIME = 30;

SceneTitle::SceneTitle( ) :
_count( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_TITLE, "title/roomb_title.png" );
	drawer->loadGraph( GRAPH_PLEASE_PUSH_BUTTON, "title/pleasepushbutton.png" );
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "bgm_maoudamashii_cyber31.wav" );
}


SceneTitle::~SceneTitle( ) {
}

Scene::NEXT SceneTitle::update( ) {
	_count++;
	draw( );
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		SoundPtr sound = Sound::getTask( );
		sound->playSE( "se_maoudamashii_system49.wav" );
		return NEXT_STAGE_SELECT;
	}
	return NEXT_CONTINUE;
}

void SceneTitle::draw( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	{
		Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - TITLE_WIDTH / 2, HEIGHT / 10 ), GRAPH_TITLE );
		drawer->setSprite( sprite );
	}
	if ( _count % DRAW_TIME < DRAW_TIME / 2 ) {
		Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - TITLE_WIDTH / 2, HEIGHT / 2 - TITLE_HEIGHT / 2 ), GRAPH_PLEASE_PUSH_BUTTON );
		drawer->setSprite( sprite );
	}
}
