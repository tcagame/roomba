#include "Camera.h"
#include "Application.h"
#include "Drawer.h"

const Vector START_CAMERA_POS = Vector( 0, 100, 50 );
const Vector START_TARGET_POS = Vector( 0, 0, 0 );

Camera::Camera( ) :
_pos( START_CAMERA_POS ),
_target( START_TARGET_POS ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCameraUp( Vector( 0, 0, 1 ) );
	drawer->setCamera( _pos, _target );
}


Camera::~Camera( ) {
}

void Camera::update( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCamera( _pos, _target );
}