#include "SceneResult.h"
#include "Drawer.h"
#include "Device.h"
#include "define.h"

static const int RESULT_WIDTH  = 1024;
static const int RESULT_HEIGHT = 256;
static const int RESULT_CENTER_X = 1450 / 2;
static const int RESULT_CENTER_Y = 180;

SceneResult::SceneResult( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_RESULT, "Result/result.png" );
}


SceneResult::~SceneResult( ) {
}

Scene::NEXT SceneResult::update( ) {
	draw( );
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		return NEXT_STAGE_SELECT;
	}
	return NEXT_CONTINUE;
}

void SceneResult::draw( ) {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( RESULT_CENTER_X - RESULT_WIDTH / 2, RESULT_CENTER_Y - RESULT_HEIGHT / 2 ), GRAPH_RESULT );
	drawer->setSprite( sprite );
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