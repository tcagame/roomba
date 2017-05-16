#include "SceneSelect.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"

static const int TITLE_WIDTH  = 1024;
static const int TITLE_HEIGHT = 256;
static const int TITLE_CENTER_X = 1450 / 2;
static const int TITLE_CENTER_Y = 180;

SceneSelect::SceneSelect( ) {
}


SceneSelect::~SceneSelect( ) {
}

Scene::NEXT SceneSelect::update( ) {
	draw( );
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		return NEXT_STAGE;
	}
	return NEXT_CONTINUE;
}

void SceneSelect::draw( ) {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( TITLE_CENTER_X - TITLE_WIDTH / 2, TITLE_CENTER_Y - TITLE_HEIGHT / 2 ), STAGE_SELECT );
	drawer->setSprite( sprite );
}
