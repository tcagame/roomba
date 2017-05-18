#include "SceneRetry.h"
#include "Application.h"
#include "define.h"
#include "Drawer.h"
#include "Device.h"
#include "Game.h"

SceneRetry::SceneRetry( int stage_num ) :
_stage( stage_num ),
_select( false ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_SELECT_MENU, "UI/UI_retry_select.png" );
	drawer->loadGraph( GRAPH_GAMEOVER, "UI/game_over.png" );
}


SceneRetry::~SceneRetry( ) {
}

Scene::NEXT SceneRetry::update( ) {
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		if ( !_select ) {
			GamePtr game = Game::getTask( );
			game->setStage( _stage );
			return NEXT_STAGE;		
		} else {
			return NEXT_TITLE;
		}
	}
	if ( device->getDirY( ) > 0 && !_ispush ) {
		_select = true;
		_ispush = true;
	}
	if ( device->getDirY( ) < 0 && !_ispush ) {
		_select = false;
		_ispush = true;
	}
	if ( device->getDirY( ) == 0 && _ispush ) {
		_ispush = false;
	}

	draw( );

	return NEXT_CONTINUE;
}

void SceneRetry::draw( ) {
	//選択画面
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	int width = app->getWindowWidth( );
	int height = app->getWindowHeight( );
	int select_y = height / 2 + 64 + _select * 32;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 0, 0 ), GRAPH_GAMEOVER ) ); // ゲームオーバー
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 64, height / 2, 0, 0, 128, 64 ), GRAPH_SELECT_MENU ) ); // リトライ文字
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 32, height / 2 + 64, 0, 64, 64, 64 ), GRAPH_SELECT_MENU ) ); // yes no
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 64, select_y, 64, 64, 64, 64 ), GRAPH_SELECT_MENU ) ); // →
}