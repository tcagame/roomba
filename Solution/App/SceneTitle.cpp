#include "SceneTitle.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"

static const int TITLE_WIDTH  = 1024;
static const int TITLE_HEIGHT = 256;
static const int TITLE_CENTER_X = 1450 / 2;
static const int TITLE_CENTER_Y = 180;
static const int DRAW_TIME = 30;


SceneTitle::SceneTitle( ) :
_count( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_TITLE, "title/roomb_title.png" );
	drawer->loadGraph( GRAPH_PLEASE_PUSH_BUTTON, "title/pleasepushbutton.png" );
}


SceneTitle::~SceneTitle( ) {
}

Scene::NEXT SceneTitle::update( ) {
	_count++;
	draw( );
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		return NEXT_STAGE_SELECT;
	}
	return NEXT_CONTINUE;
}

void SceneTitle::draw( ) {
	DrawerPtr drawer = Drawer::getTask( );
	{
		Drawer::Sprite sprite( Drawer::Transform( TITLE_CENTER_X - TITLE_WIDTH / 2, TITLE_CENTER_Y - TITLE_HEIGHT / 2 ), GRAPH_TITLE );
		drawer->setSprite( sprite );
	}
	if ( _count % DRAW_TIME < DRAW_TIME / 2 ) {
		Drawer::Sprite sprite( Drawer::Transform( TITLE_CENTER_X - TITLE_WIDTH / 2, TITLE_CENTER_Y - TITLE_HEIGHT / 2 + TITLE_HEIGHT * 1.5 ), GRAPH_PLEASE_PUSH_BUTTON );
		drawer->setSprite( sprite );
	}
}
