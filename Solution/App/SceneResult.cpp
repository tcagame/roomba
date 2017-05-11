#include "SceneResult.h"



SceneResult::SceneResult( ) {
}


SceneResult::~SceneResult( ) {
}
	/*if ( _state == STATE_GAME_CLEAR ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 700, 400, "‚°[‚Þ‚­‚è‚ " );
	}
	if ( _state == STATE_SELECT_RETRY ) {
		//‘I‘ð‰æ–Ê
		DrawerPtr drawer = Drawer::getTask( );
		ApplicationPtr app = Application::getInstance( );
		int width = app->getWindowWidth( );
		int height = app->getWindowHeight( );
		int select_y = height / 2 + 32 + _select * 64;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 128, height / 2 - 128, 0, 0, 256, 256 ), GRAPH_SELECT_MENU ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 64, height / 2, 256, 0, 128, 128 ), GRAPH_SELECT_MENU ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 64, height / 2 + 64, 256, 0, 128, 128 ), GRAPH_SELECT_MENU ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 128, select_y, 384, 0, 64, 64 ), GRAPH_SELECT_MENU ) );
	}*/