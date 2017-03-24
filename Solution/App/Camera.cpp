#include "Camera.h"
#include "Application.h"
#include "Drawer.h"
#include "Mouse.h"
#include "Roomba.h"

const Vector START_CAMERA_POS = Vector( 0, -100, 50 );
const Vector START_TARGET_POS = Vector( 0, 0, 0 );
const int ROTE_SPEED = 10;
const int MAX_LENGTH = 200;

Camera::Camera( ) :
_pos( START_CAMERA_POS ),
_target( START_TARGET_POS ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCameraUp( Vector( 0, 0, 1 ) );
	drawer->setCamera( _pos, _target );
}


Camera::~Camera( ) {
}

void Camera::update( RoombaPtr target ) {
	_target = target->getCentralPos( );
	move( );
	Vector vec = _pos - _target;

	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCamera( _pos, _target );
}

void Camera::move( ) {
	MousePtr mouse = Mouse::getTask( );
	if ( mouse->isHoldLeftButton( ) ) {
		//‰ñ“]
		Vector mouse_vec = mouse->getPos( ) - _before_mouse_pos;
		Vector camera_dir = _pos - _target;
		camera_dir.z = 0;
		double angle = mouse_vec.angle( Vector( 0, -1, 0 ) );
		Vector axis = mouse_vec.cross( Vector( 0, -1, 0 ) );
		Matrix mat = Matrix::makeTransformRotation( axis, angle );
		Vector vec = mat.multiply( camera_dir );
		_pos += vec.normalize( ) * ROTE_SPEED;
		Vector distance = _pos - _target;
		if ( distance.getLength( ) > MAX_LENGTH ) {
			_pos = _target + distance.normalize( ) * MAX_LENGTH;
		}
	}
	_before_mouse_pos = mouse->getPos( );
}

Vector Camera::getDir( ) const {
	Vector dir = ( _target - _pos ).normalize( );
	dir.z = 0;
	return dir;
}