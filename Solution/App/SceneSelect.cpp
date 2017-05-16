#include "SceneSelect.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"

static const int SELECT_WIDTH  = 1024;
static const int SELECT_HEIGHT = 256;
static const int SELECT_CENTER_X = 1450 / 2;
static const int SELECT_CENTER_Y = 180;
static const int PITCH = 150; 

SceneSelect::SceneSelect( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( STAGE_SELECT_ROGO, "select/select.png");
	drawer->loadGraph( STAGE_SELECT_1, "select/stage1.png");
	drawer->loadGraph( STAGE_SELECT_2, "select/stage2.png");
	drawer->loadGraph( STAGE_SELECT_3, "select/stage3.png");
	drawer->loadGraph( STAGE_SELECTER, "select/selecter.png");
}


SceneSelect::~SceneSelect( ) {
}

Scene::NEXT SceneSelect::update( ) {
	draw( );
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		return NEXT_STAGE;
	}
	if ( device->getDirY( ) < 0 ) {
		_select++;
	}
	if ( device->getDirY( ) > 0 ) {
		_select--;
	}
	return NEXT_CONTINUE;
}

void SceneSelect::draw( ) {
	drawRogo( );
	drawSelect( );
}

void SceneSelect::drawRogo( ) {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( SELECT_CENTER_X - SELECT_WIDTH / 2, SELECT_CENTER_Y - SELECT_HEIGHT / 2 ), STAGE_SELECT_ROGO );
	drawer->setSprite( sprite );
}

void SceneSelect::drawSelect( ) {
	DrawerPtr drawer = Drawer::getTask( );
	int x = 400;
	int y = 550;
	
	// –îˆó
	Drawer::Sprite sprite( Drawer::Transform( 150, 300 ), STAGE_SELECTER );
	drawer->setSprite( sprite );

	// stage
	for ( int i = 0; i < 3; i++ ) {
		Drawer::Sprite sprite( Drawer::Transform( x - i * PITCH, y - i * PITCH ), STAGE_SELECT_3 - (GRAPH)( i + _select % 3 ) );
		drawer->setSprite( sprite );
	}
}