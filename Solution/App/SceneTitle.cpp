#include "SceneTitle.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"
#include "Application.h"
#include "Sound.h"

static const int TITLE_WIDTH  = 1024;
static const int TITLE_HEIGHT = 256;
static const int DRAW_TIME = 100;
static const int BRANK = 250;
static const int CIRCLE_ANIME_FLAME = 1;
static const int MAX_TITLE_COUNT = 34 * 3;
static const int MAX_CHOICE_COUNT = 24 * CIRCLE_ANIME_FLAME;

SceneTitle::SceneTitle( ) :
_count( 0 ),
_choice_count( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_MANUAL, "title/ok_manual.png" );
	drawer->loadGraph( GRAPH_CONTROLLER_NEUTRAL, "controller/neutral.png" );
	drawer->loadGraph( GRAPH_CONTROLLER_ROTATION, "controller/rotation.png" );
	drawer->loadGraph( GRAPH_BG, "title/title_bg.png" );
	drawer->loadGraph( GRAPH_OK, "UI/ok.png" );
	drawer->loadGraph( GRAPH_TITLE, "title/roomb_title_2.png" );
	drawer->loadGraph( GRAPH_PLEASE_PUSH_BUTTON, "title/pleasepushbutton.png" );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle_blue1.png" );
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "titleBGM.wav" );
	resetCount( );
}


SceneTitle::~SceneTitle( ) {
}

Scene::NEXT SceneTitle::update( ) {
	_count++;
	draw( );
	SoundPtr sound = Sound::getTask( );
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

	// サークルカウント
	if ( ( right_stick.y > 0 && left_stick.y < 0 ) ||
		 ( right_stick.y < 0 && left_stick.y > 0 ) ) {
		_choice_count++;
		if ( _choice_count == 1 ) {
			sound->playSE( "circleSE.wav" );
		}
	} else {
		_choice_count = 0;
		sound->stopSE( "circleSE.wav" );
	}
	return NEXT_CONTINUE;
}

void SceneTitle::draw( ) {
	drawFadeBg( );
	drawBg( );
	drawManual( );
	drawTitle( );
	//drawPlease( );
	drawController( );
	drawCircle( );
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		drawFadeIn( );
	} else {
		drawFadeOut( );
	}
}

void SceneTitle::drawBg( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( 0, 0, 0, 0, 1920, 1080, WIDTH, HEIGHT ), GRAPH_BG );
	drawer->setSprite( sprite );
}

void  SceneTitle::drawTitle( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	const int ANIME_TIME = 300;
	double blend = 1;
	const int now = _count % ANIME_TIME;

	if ( now < ANIME_TIME / 2 ) {
		blend = (double)( _count % ANIME_TIME ) / 100;
	} else {
		blend = 1 - ( (double)( _count % ( ANIME_TIME / 2 ) ) / 100 );
	}
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - TITLE_WIDTH / 2, HEIGHT / 2 - TITLE_HEIGHT, 0, 0, TITLE_WIDTH, TITLE_HEIGHT ), GRAPH_TITLE, Drawer::BLEND_ALPHA, blend );
	drawer->setSprite( sprite );
	
}

void SceneTitle::drawPlease( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int tx = 122;
	const int ty = 116;
	const int tx2 = 269;
	const int ty2 = 52;

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Transform trans( WIDTH / 2 - tx2 / 2, HEIGHT / 2, tx, ty, tx2, ty2  );
	Drawer::Sprite sprite( trans, GRAPH_PLEASE_PUSH_BUTTON );
	drawer->setSprite( sprite );
}

void SceneTitle::drawController( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int CONTROLLER_SIZE = 512;

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Transform trans( WIDTH / 2 - CONTROLLER_SIZE / 4, HEIGHT * 3 / 6, 0, 0, CONTROLLER_SIZE, CONTROLLER_SIZE, WIDTH / 2 - CONTROLLER_SIZE / 4 + CONTROLLER_SIZE / 2, HEIGHT * 3 / 6 + CONTROLLER_SIZE / 2 );
	//Drawer::Transform trans( WIDTH / 2 - CONTROLLER_SIZE / 4, HEIGHT / 2 + CONTROLLER_SIZE / 4 + CONTROLLER_SIZE * 2 / 6, 0, 0, CONTROLLER_SIZE, CONTROLLER_SIZE, WIDTH / 2 - CONTROLLER_SIZE / 4 + CONTROLLER_SIZE / 2, HEIGHT / 2 + CONTROLLER_SIZE / 2 + CONTROLLER_SIZE / 4 + CONTROLLER_SIZE  * 2 / 6 );
	if ( _count % DRAW_TIME < DRAW_TIME * 2 / 3 ) {
		Drawer::Sprite sprite( trans, GRAPH_CONTROLLER_ROTATION );
		drawer->setSprite( sprite );
	} else {
		Drawer::Sprite sprite( trans, GRAPH_CONTROLLER_NEUTRAL );
		drawer->setSprite( sprite );
	}
	int tx = 168;
	int ty = 366;
	int tw = 167;
	int th = 74;
	Drawer::Transform trans2( WIDTH / 2 - tw / 2, HEIGHT * 6 / 8, tx, ty,  tw,  th, WIDTH / 2 + tw / 2,  HEIGHT * 6 / 8 + th );
	Drawer::Sprite sprite( trans2, GRAPH_OK );
	drawer->setSprite( sprite );
	
}

void SceneTitle::drawCircle( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	
	const int CIRCLE_SIZE = 100;
	int idx = _choice_count / CIRCLE_ANIME_FLAME;
	if ( idx > 24 || getFadeOutCount( ) != MAX_FADE_COUNT  ) {
		idx = 24;
	}
	int tx = idx % 5;
	int ty = idx / 5;
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, HEIGHT * 6 / 8 - 16, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	drawer->setSprite( sprite );
}

void SceneTitle::drawManual( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int tw = 1024;
	const int th = 128;

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Transform trans( WIDTH / 2 - tw / 2, HEIGHT * 5 / 6, 0, 0, tw, th  );
	Drawer::Sprite sprite( trans, GRAPH_MANUAL );
	drawer->setSprite( sprite );
}
