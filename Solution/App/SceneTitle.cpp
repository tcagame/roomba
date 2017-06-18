#include "SceneTitle.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"
#include "Application.h"
#include "Sound.h"

static const int TITLE_WIDTH  = 1024;
static const int TITLE_HEIGHT = 256;
static const int DRAW_TIME = 30;
static const int BRANK = 250;
static const int CIRCLE_ANIME_FLAME = 4;
static const int MAX_CHOICE_COUNT = 24 * CIRCLE_ANIME_FLAME;

SceneTitle::SceneTitle( ) :
_count( 0 ),
_choice_count( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_TITLE, "title/roomb_title.png" );
	drawer->loadGraph( GRAPH_PLEASE_PUSH_BUTTON, "title/pleasepushbutton.png" );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle.png" );
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "bgm_maoudamashii_cyber31.wav" );
	resetCount( );
}


SceneTitle::~SceneTitle( ) {
}

Scene::NEXT SceneTitle::update( ) {
	_count++;
	draw( );
	DevicePtr device = Device::getTask( );
	Vector right_stick = Vector( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick = Vector( device->getDirX( ), device->getDirY( ) );
	// フェードイン
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		addFadeInCount( );
		return NEXT_CONTINUE;
	}
	// フェードアウト
	if ( _choice_count > MAX_CHOICE_COUNT ||
		 getFadeOutCount( ) < MAX_FADE_COUNT ) {
		subFadeOutCount( );
		if ( getFadeOutCount( ) < 0 ) {
			return NEXT_STAGE_SELECT;
		}
	}
	if ( right_stick.y > 0 && left_stick.y < 0 ) {
		_choice_count++;
		SoundPtr sound = Sound::getTask( );
		sound->playSE( "se_maoudamashii_system45.wav" );
	} else {
		_choice_count = 0;
	}
	return NEXT_CONTINUE;
}

void SceneTitle::draw( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	{
		Drawer::Sprite sprite( Drawer::Transform( BRANK, HEIGHT / 10, 0, 0, TITLE_WIDTH, TITLE_HEIGHT, WIDTH - BRANK, HEIGHT * 4 / 10 ), GRAPH_TITLE );
		drawer->setSprite( sprite );
	}
	if ( _count % DRAW_TIME < DRAW_TIME / 2 ) {
		Drawer::Sprite sprite( Drawer::Transform( BRANK, HEIGHT * 5 / 10, 0, 0, TITLE_WIDTH, TITLE_HEIGHT, WIDTH - BRANK, HEIGHT * 5 / 10 + 256 ), GRAPH_PLEASE_PUSH_BUTTON );
		drawer->setSprite( sprite );
	}
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		drawFadeIn( );
	} else {
		drawFadeOut( );
	}
	drawCircle( );
}

void SceneTitle::drawCircle( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	
	const int CIRCLE_SIZE = 100;
	int idx = _choice_count / CIRCLE_ANIME_FLAME;
	if ( idx > 25 ) {
		idx = 25;
	}
	int tx = idx % 4;
	int ty = idx / 4;
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, HEIGHT / 2 - CIRCLE_SIZE / 2, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	drawer->setSprite( sprite );
}	
