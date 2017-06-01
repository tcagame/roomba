#include "Camera.h"
#include "Application.h"
#include "Drawer.h"

Camera::Camera( Vector pos, Vector target ) :
_pos( pos ),
_target( target ) {
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

void Camera::setPos( Vector pos ) {
	_pos = pos;
}

void Camera::setTarget( Vector target ) {
	_target = target;
}

Vector Camera::getPos( ) const {
	return _pos;
}

Vector Camera::getTarget( ) const {
	return _target;
}

Vector Camera::getDir( ) const {
	return ( _target - _pos ).normalize( );
}