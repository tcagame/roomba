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
		return NEXT_TITLE;
	}
	return NEXT_CONTINUE;
}

void SceneResult::draw( ) {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( RESULT_CENTER_X - RESULT_WIDTH / 2, RESULT_CENTER_Y - RESULT_HEIGHT / 2 ), GRAPH_RESULT );
	drawer->setSprite( sprite );
}
