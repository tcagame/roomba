#include "SceneGameOver.h"
#include "Application.h"
#include "define.h"
#include "Drawer.h"
#include "Device.h"
#include "Game.h"
#include "Sound.h"

static const int GRAPH_GAMEOVER_WIDTH  = 1024;
static const int GRAPH_GAMEOVER_HEIGHT = 256;
static const int BRANK = 300;
static const int CIRCLE_ANIME_FLAME = 4;
static const int MAX_CHOICE_COUNT = 25 * CIRCLE_ANIME_FLAME;
static const int THICK_FRAME_SIZE = 57;
static const int TRIANGLE_CENTER_X = 312 / 2;
static const int SELECT_SIZE  = 512;

SceneGameOver::SceneGameOver( int stage_num ) :
_stage( stage_num ),
_choice_count( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle.png" );
	drawer->loadGraph( GRAPH_STAGE_SELECT, "select/Stage Select.png" );
	drawer->loadGraph( GRAPH_GAMEOVER, "UI/game_over.png" );
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "bgm_maoudamashii_cyber29.wav" );
}


SceneGameOver::~SceneGameOver( ) {
}

Scene::NEXT SceneGameOver::update( ) {
	SoundPtr sound = Sound::getTask( );
	DevicePtr device = Device::getTask( );
	Vector right_stick = Vector( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick = Vector( device->getDirX( ), device->getDirY( ) );
// フェードイン
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		addFadeInCount( );
	}
	// フェードアウト
	if ( _choice_count > MAX_CHOICE_COUNT ||
		getFadeOutCount( ) < MAX_FADE_COUNT ) {
		subFadeOutCount( );
		if ( getFadeOutCount( ) < 0 ) {
			return NEXT_TITLE;
		}
	}
	if ( right_stick.y > 0 && left_stick.y < 0 ) {
		_choice_count++;
		sound->playSE( "se_maoudamashii_system45.wav" );
	} else {
		_choice_count = 0;
	}

	draw( );

	return NEXT_CONTINUE;
}

void SceneGameOver::draw( ) {
	//選択画面
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( WIDTH / 2 - GRAPH_GAMEOVER_WIDTH / 2, HEIGHT / 2 - GRAPH_GAMEOVER_HEIGHT / 2 , 0, 0 ), GRAPH_GAMEOVER ) ); // ゲームオーバー
	drawFrame( );
	drawFadeBg( );
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		drawFadeIn( );
	} else {
		drawFadeOut( );
	}
	drawCircle( );
}

void SceneGameOver::drawFrame( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );

	//黄色い太枠
	const int T_SX[ 4 ] = { 0, WIDTH - THICK_FRAME_SIZE, 0, WIDTH - THICK_FRAME_SIZE };
	const int T_SY[ 4 ] = { 0, 0, HEIGHT - THICK_FRAME_SIZE, HEIGHT - THICK_FRAME_SIZE };
	const int T_TX[ 4 ] = { 4, SELECT_SIZE - THICK_FRAME_SIZE - 6, 6, SELECT_SIZE - THICK_FRAME_SIZE - 9 };
	const int T_TY[ 4 ] = { 4, 4, SELECT_SIZE - THICK_FRAME_SIZE - 4, SELECT_SIZE - THICK_FRAME_SIZE - 4 };

	for ( int i = 0; i < 4; i++ ) {
		Drawer::Transform trans( T_SX[ i ], T_SY[ i ], T_TX[ i ], T_TY[ i ], THICK_FRAME_SIZE, THICK_FRAME_SIZE );
		Drawer::Sprite sprite( trans, GRAPH_STAGE_SELECT );
		drawer->setSprite( sprite );
	}

	//白い細枠
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

void SceneGameOver::drawCircle( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	
	const int CIRCLE_SIZE = 100;
	const int idx = _choice_count / CIRCLE_ANIME_FLAME;
	int tx = idx % 4;
	int ty = idx / 4;
	if ( ty > 6 ) {
		ty = 6;
		if ( tx > 1 ) {
			tx = 1;
		}
	}
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, HEIGHT / 2 - CIRCLE_SIZE / 2, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	drawer->setSprite( sprite );
}
