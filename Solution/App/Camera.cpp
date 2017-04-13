#include "Camera.h"
#include "Application.h"
#include "Drawer.h"
#include "Mouse.h"
#include "Roomba.h"

const Vector START_CAMERA_POS = Vector( 0, -100, 75 );
const Vector START_TARGET_POS = Vector( 0, 0, 0 );
const double ROTE_SPEED = PI / 90;
const int CAMERA_LENGTH = 200;

Camera::Camera( ) :
_pos( START_CAMERA_POS ),
_target( START_TARGET_POS ),
_mouse_x( 0 ) {
	_dir = _target - _pos;
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCameraUp( Vector( 0, 0, 1 ) );
	drawer->setCamera( _pos, _target );
}


Camera::~Camera( ) {
}

void Camera::update( RoombaPtr target ) {
	_target = target->getCentralPos( );
	move( );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCamera( _pos, _target );
}

void Camera::move( ) {
	//‰ñ“]
	int axis = 0;
	MousePtr mouse = Mouse::getTask( );
	int mouse_x = (int)mouse->getPos( ).x;
	if ( mouse->isHoldLeftButton( ) ) {
		if ( mouse_x > _mouse_x ) {
			axis = -1;
		}
		if ( mouse_x < _mouse_x ) {
			axis = 1;
		}
	}
	_mouse_x = mouse_x;

	if ( axis != 0 ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, axis ), ROTE_SPEED );
		_dir = mat.multiply( _dir );
	}
	_pos = _target - _dir.normalize( ) * CAMERA_LENGTH;
}

Vector Camera::getDir( ) const {
	return _dir;
}

void Camera::reset( ) {
	_pos = START_CAMERA_POS;
	_target = START_TARGET_POS;
	_dir = _target - _pos;
}
