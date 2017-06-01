#include "AppCamera.h"
#include "define.h"
#include "Mouse.h"
#include "Device.h"
#include "Roomba.h"

const double HEIGHT = 0.4;
const double ROTE_SPEED = PI / 40;
const int CAMERA_LENGTH = (int)( 40 * WORLD_SCALE );

AppCamera::AppCamera( RoombaPtr roomba ) :
_roomba( roomba ),
_mouse_x( 0 ),
Camera( roomba->getCentralPos( ) -  getCalcDir( roomba->getDir( ), HEIGHT ) * CAMERA_LENGTH, roomba->getCentralPos( ) ) {
	Vector dir = getCalcDir( roomba->getDir( ), HEIGHT );
	setDir( dir );
}


AppCamera::~AppCamera( ) {

}

void AppCamera::move( ) {
	setTarget( _roomba->getCentralPos( ) );
	//‰ñ“]
	Vector dir = getCalcDir( _roomba->getDir( ), HEIGHT );
	//dir‚Éz‚ð‘«‚µ‚½‚Æ‚«‚É
	Vector pos = getTarget( ) - dir.normalize( ) * CAMERA_LENGTH;
	setDir( dir );
	setPos( pos );
}

void AppCamera::reset( ) {
	setPos( _roomba->getCentralPos( ) - getCalcDir( _roomba->getDir( ), HEIGHT ) * CAMERA_LENGTH );
	setTarget( _roomba->getCentralPos( ) );
	setDir( getCalcDir( _roomba->getDir( ), HEIGHT ) );
}


Vector AppCamera::getCalcDir( Vector dir, double HEIGHT ) const {
	dir.z = -HEIGHT;
	return dir.normalize( );
}
