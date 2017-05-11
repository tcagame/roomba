#include "SceneTitle.h"
#include "Device.h"



SceneTitle::SceneTitle( ) {
}


SceneTitle::~SceneTitle( ) {
}

Scene::NEXT SceneTitle::update( ) {
	DevicePtr device = Device::getTask( );
	if ( device->getButton( ) && BUTTON_D ) {
		return NEXT_STAGE;
	}
	return NEXT_CONTINUE;
}
