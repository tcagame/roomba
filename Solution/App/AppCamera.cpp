#include "AppCamera.h"
#include "define.h"
#include "Mouse.h"
#include "Device.h"
#include "Roomba.h"

const Vector START_CAMERA_POS = Vector( 0, -100, 75 );
const Vector START_TARGET_POS = Vector( 0, 0, 0 );
const double ROTE_SPEED = PI / 40;
const int CAMERA_LENGTH = (int)( 100 * WORLD_SCALE );

AppCamera::AppCamera( RoombaPtr roomba ) :
Camera( START_CAMERA_POS, START_TARGET_POS ),
_roomba( roomba ),
_mouse_x( 0 ) {

}


AppCamera::~AppCamera( ) {

}

void AppCamera::move( ) {
	setTarget( _roomba->getCentralPos( ) );
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

	DevicePtr device = Device::getTask( );
	if ( device->getButton( ) & BUTTON_F ) {
		axis = -1;
	}
	if ( device->getButton( ) & BUTTON_E ) {
		axis = 1;
	}

	if ( axis != 0 ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, axis ), ROTE_SPEED );
		setDir( mat.multiply( getDir( ) ) );
	}
	setPos( getTarget( ) - getDir( ).normalize( ) * CAMERA_LENGTH );
}

void AppCamera::reset( ) {
	setPos( START_CAMERA_POS );
	setTarget( START_TARGET_POS );
	setDir( _target - _pos );
}
