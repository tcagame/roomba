#include "AppCamera.h"
#include "define.h"
#include "Mouse.h"
#include "Device.h"
#include "Roomba.h"

const Vector START_DIR( 0, 4, -2 );
const double ROTE_SPEED = PI / 40;
const int CAMERA_LENGTH = (int)( 20 * WORLD_SCALE );

AppCamera::AppCamera( RoombaPtr roomba ) :
_roomba( roomba ),
_mouse_x( 0 ),
Camera( roomba->getCentralPos( ) - START_DIR.normalize( ) * CAMERA_LENGTH, roomba->getCentralPos( ) ) {
	setDir( START_DIR );
}


AppCamera::~AppCamera( ) {

}

void AppCamera::update( ) {
	move( );
	Vector dir = getDir( ).normalize( );
	Vector pos = getPos( );
	Vector target = getTarget( );
	double length = (target - pos ).getLength( );
	if ( pos.x < 0 ) {
		pos.z -= -pos.x;
		pos.x = 0;
	}
	if ( pos.x > STAGE_WIDTH_NUM * WORLD_SCALE - 1 ) {
		pos.z += pos.x - STAGE_WIDTH_NUM * WORLD_SCALE - 1;
		pos.x = STAGE_WIDTH_NUM * WORLD_SCALE - 1;
	}
	if ( pos.y < 0 ) {
		pos.z -= pos.y;
		pos.y = 0;
	}
	if ( pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE ) {
		pos.z += pos.y - STAGE_HEIGHT_NUM * WORLD_SCALE - 1;
		pos.y = STAGE_HEIGHT_NUM * WORLD_SCALE - 1;
	}
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setCamera( pos, target );
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

	Vector dir = getDir( );
	Vector pos = getPos( );
	Vector target = getTarget( );
	if ( axis != 0 ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, axis ), ROTE_SPEED );
		dir = mat.multiply( dir ).normalize( );
		setDir( dir );
	}
	pos = target - dir.normalize( ) * CAMERA_LENGTH;
	setPos( pos );
}

void AppCamera::reset( ) {
	setPos( _roomba->getCentralPos( ) - START_DIR.normalize( ) * CAMERA_LENGTH );
	setTarget( _roomba->getCentralPos( ) );
	setDir( START_DIR );
}
