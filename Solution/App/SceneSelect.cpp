#include "SceneSelect.h"
#include "Device.h"
#include "define.h"

SceneSelect::SceneSelect( ) {
}


SceneSelect::~SceneSelect( ) {
}

Scene::NEXT SceneSelect::update( ) {
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		return NEXT_STAGE;
	}
	return NEXT_CONTINUE;
}

