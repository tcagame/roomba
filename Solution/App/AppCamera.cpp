#include "AppCamera.h"
#include "define.h"
#include "Mouse.h"
#include "Device.h"
#include "Roomba.h"

const Vector START_DIR( 0, 3, -2 );
const double ROTE_SPEED = PI / 40;
const int CAMERA_LENGTH = (int)( 40 * WORLD_SCALE );

AppCamera::AppCamera( RoombaPtr roomba ) :
_roomba( roomba ),
_mouse_x( 0 ),
Camera( roomba->getCentralPos( ) - START_DIR.normalize( ) * CAMERA_LENGTH, roomba->getCentralPos( ) ) {
	setDir( START_DIR );
}


AppCamera::~AppCamera( ) {

}

void AppCamera::move( ) {
	setTarget( _roomba->getCentralPos( ) );
	//‰ñ“]
	Vector dir = _roomba->getDir( );
	dir.z = getDir( ).z;
	//dir‚Éz‚ð‘«‚µ‚½‚Æ‚«‚É
	double ratio = fabs( ( 1.0 / 3.0 * 2 ) / ( dir.x * dir.x + dir.y * dir.y ) );//x+y : z == 2 : 1
	dir.x *= ratio;
	dir.y *= ratio;
	Vector pos = getTarget( ) - dir.normalize( ) * CAMERA_LENGTH;
	setDir( dir );
	setPos( pos );
}

void AppCamera::reset( ) {
	setPos( _roomba->getCentralPos( ) - START_DIR.normalize( ) * CAMERA_LENGTH );
	setTarget( _roomba->getCentralPos( ) );
	setDir( START_DIR );
}