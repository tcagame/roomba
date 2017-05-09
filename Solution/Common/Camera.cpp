#include "Camera.h"
#include "Application.h"
#include "Drawer.h"

Camera::Camera( Vector pos, Vector target ) :
_mouse_x( 0 ),
_pos( pos ),
_target( target ) {
	_dir = target - pos;
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCameraUp( Vector( 0, 0, 1 ) );
	drawer->setCamera( pos, target );
}


Camera::~Camera( ) {
}

void Camera::update( ) {
	move( );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCamera( _pos, _target );
}

void Camera::move( ) {
}

Vector Camera::getDir( ) const {
	return _dir;
}

void Camera::setPos( Vector pos ) {
	_pos = pos;
}

void Camera::setTarget( Vector target ) {
	_target = target;
}

void Camera::setDir( Vector dir ) {
	_dir = dir;
}

Vector Camera::getPos( ) {
	return _pos;
}

Vector Camera::getTarget( ) {
	return _target;
}