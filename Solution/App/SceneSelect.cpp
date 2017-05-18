#include "SceneSelect.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"

static const int SELECT_WIDTH  = 1024;
static const int SELECT_HEIGHT = 256;
static const int SELECT_CENTER_X = 1450 / 2;
static const int SELECT_CENTER_Y = 180;
static const int PITCH = 150; 

SceneSelect::SceneSelect( ) :
_select( 1 ),
_ispush( false ) {
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
	if ( device->getDirY( ) > 0 && !_ispush ) {
		_select++;
		_ispush = true;
	}
	if ( device->getDirY( ) < 0 && !_ispush ) {
		_select--;
		_ispush = true;
	}
	if ( device->getDirY( ) == 0 && _ispush ) {
		_ispush = false;
	}
	_select = abs( _select ) % 3;
	if ( _select == 0 ) {
		_select = 3;
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
	int x = 100;
	int y = 250;
	
	// –îˆó
	Drawer::Sprite sprite( Drawer::Transform( 150, 300 ), STAGE_SELECTER );
	drawer->setSprite( sprite );

	GRAPH graph = (GRAPH)( STAGE_SELECT_1 + ( _select - 1 ) );

	// stage
	for ( int i = 0; i < 3; i++, graph = (GRAPH)( graph + 1 ) ) {
		if ( graph == (GRAPH)( STAGE_SELECT_3 + 1 ) ) {
			graph = STAGE_SELECT_1;
		}
		Drawer::Sprite sprite( Drawer::Transform( x + i * PITCH, y + i * PITCH ), graph );
		drawer->setSprite( sprite );
	}
}