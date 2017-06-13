#include "SceneRetry.h"
#include "Application.h"
#include "define.h"
#include "Drawer.h"
#include "Device.h"
#include "Game.h"
#include "Sound.h"

static const int GRAPH_GAMEOVER_WIDTH  = 1024;
static const int GRAPH_GAMEOVER_HEIGHT = 256;
static const int BRANK = 300;

SceneRetry::SceneRetry( int stage_num ) :
_stage( stage_num ),
_select( false ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_SELECT_MENU, "UI/UI_retry_select.png" );
	drawer->loadGraph( GRAPH_GAMEOVER, "UI/game_over.png" );
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "bgm_maoudamashii_cyber29.wav" );
}


SceneRetry::~SceneRetry( ) {
}

Scene::NEXT SceneRetry::update( ) {
	SoundPtr sound = Sound::getTask( );
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		sound->playSE( "se_maoudamashii_system45.wav" );
		if ( !_select ) {
			GamePtr game = Game::getTask( );
			game->setStage( _stage );
			return NEXT_STAGE;		
		} else {
			return NEXT_TITLE;
		}
	}
	if ( device->getDirY( ) > 0 && !_ispush ) {
		sound->playSE( "se_maoudamashii_system43.wav" );
		_select = true;
		_ispush = true;
	}
	if ( device->getDirY( ) < 0 && !_ispush ) {
		sound->playSE( "se_maoudamashii_system43.wav" );
		_select = false;
		_ispush = true;
	}
	if ( device->getDirY( ) == 0 && _ispush ) {
		sound->playSE( "se_maoudamashii_system43.wav" );
		_ispush = false;
	}

	draw( );

	return NEXT_CONTINUE;
}

void SceneRetry::draw( ) {
	//選択画面
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( BRANK, HEIGHT / 10, 0, 0, GRAPH_GAMEOVER_WIDTH, GRAPH_GAMEOVER_HEIGHT, WIDTH - BRANK, HEIGHT * 3 / 10 ), GRAPH_GAMEOVER ) ); // ゲームオーバー
	int sy = HEIGHT / 2;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( BRANK * 2, sy, 0, 0, 128, 64, WIDTH - BRANK * 2, sy + HEIGHT / 10 ), GRAPH_SELECT_MENU ) ); // リトライ文字
	sy += HEIGHT / 10;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( BRANK * 2, sy, 0, 64, 64, 64, WIDTH - BRANK * 2, sy + HEIGHT / 5 ), GRAPH_SELECT_MENU ) ); // yes no
	sy += HEIGHT / 50;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( BRANK * 2 - WIDTH / 10, sy + _select * HEIGHT / 10, 64, 64, 64, 64, BRANK * 2, sy + HEIGHT / 10 + _select * HEIGHT / 10 ), GRAPH_SELECT_MENU ) ); // →
}