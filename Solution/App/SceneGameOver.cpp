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
static const int CIRCLE_ANIME_FLAME = 1;
static const int MAX_CHOICE_COUNT = 25 * CIRCLE_ANIME_FLAME;
static const int THICK_FRAME_SIZE = 57;
static const int TRIANGLE_CENTER_X = 312 / 2;
static const int SELECT_SIZE  = 512;
static const int DRAW_TIME = 100;


SceneGameOver::SceneGameOver( int stage_num ) :
_stage( stage_num ),
_choice_count( 0 ),
_count( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_CONTROLLER_NEUTRAL, "controller/neutral.png" );
	drawer->loadGraph( GRAPH_CONTROLLER_ROTATION, "controller/rotation.png" );
	drawer->loadGraph( GRAPH_OK, "UI/game_over_ok.png" );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle_red1.png" );
	drawer->loadGraph( GRAPH_STAGE_SELECT, "select/Stage Select.png" );
	drawer->loadGraph( GRAPH_GAME_OVER, "UI/game_over.png" );
	drawer->loadGraph( GRAPH_FRAME, "UI/Frame.png" );
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "GameOverBGM.wav" );
}


SceneGameOver::~SceneGameOver( ) {
}

Scene::NEXT SceneGameOver::update( ) {
	_count++;
	SoundPtr sound = Sound::getTask( );
	DevicePtr device = Device::getTask( );
	Vector right_stick = Vector( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick = Vector( device->getDirX( ), device->getDirY( ) );
	draw( );

// �t�F�[�h�C��
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		addFadeInCount( );
		return NEXT_CONTINUE;
	}
	// �t�F�[�h�A�E�g
	if ( _choice_count > MAX_CHOICE_COUNT ||
		getFadeOutCount( ) < MAX_FADE_COUNT ) {
		subFadeOutCount( );
		if ( getFadeOutCount( ) < 0 ) {
			return NEXT_TITLE;
		}
	}

	// �T�[�N���J�E���g
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

void SceneGameOver::draw( ) {
	drawFadeBg( );
	drawResult( );
	drawGameOver( );
	drawFrame( );
	drawCircle( );
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		drawFadeIn( );
	} else {
		drawFadeOut( );
	}
}

void SceneGameOver::drawResult( ) {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int RESULT_WIDTH = 169;
	const int RESULT_HEIGHT = 37;

	Drawer::Transform trans( WIDTH / 2 - RESULT_WIDTH / 2, HEIGHT / 3, 427, 50, RESULT_WIDTH, RESULT_HEIGHT );
	Drawer::Sprite sprite( trans, GRAPH_GAME_OVER );
	drawer->setSprite( sprite );
}

void SceneGameOver::drawGameOver( ) {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int GAMEOVER_WIDTH = 655;
	const int GAMEOVER_HEIGHT = 72;

	Drawer::Transform trans( WIDTH / 2 - GAMEOVER_WIDTH / 2, HEIGHT / 2 - GAMEOVER_HEIGHT / 2, 192, 114, GAMEOVER_WIDTH, GAMEOVER_HEIGHT );
	Drawer::Sprite sprite( trans, GRAPH_GAME_OVER );
	drawer->setSprite( sprite );
}

void SceneGameOver::drawFrame( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );

	//���F�����g
	const int T_SX[ 4 ] = { 0, WIDTH - THICK_FRAME_SIZE, 0, WIDTH - THICK_FRAME_SIZE };
	const int T_SY[ 4 ] = { 0, 0, HEIGHT - THICK_FRAME_SIZE, HEIGHT - THICK_FRAME_SIZE };
	const int T_TX[ 4 ] = { 4, SELECT_SIZE - THICK_FRAME_SIZE - 6, 6, SELECT_SIZE - THICK_FRAME_SIZE - 9 };
	const int T_TY[ 4 ] = { 4, 4, SELECT_SIZE - THICK_FRAME_SIZE - 4, SELECT_SIZE - THICK_FRAME_SIZE - 4 };

	for ( int i = 0; i < 4; i++ ) {
		Drawer::Transform trans( T_SX[ i ], T_SY[ i ], T_TX[ i ], T_TY[ i ], THICK_FRAME_SIZE, THICK_FRAME_SIZE );
		Drawer::Sprite sprite( trans, GRAPH_FRAME );
		drawer->setSprite( sprite );
	}

	//�����טg
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
	int idx = _choice_count / CIRCLE_ANIME_FLAME;
	if ( idx > 25 || getFadeOutCount( ) != MAX_FADE_COUNT  ) {
		idx = 25;
	}
	int tx = idx % 5;
	int ty = idx / 5;

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, HEIGHT / 2 - 74 + CIRCLE_SIZE * 3 / 5, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	drawer->setSprite( sprite );
}