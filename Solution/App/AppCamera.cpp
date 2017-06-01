#include "AppCamera.h"
#include "define.h"
#include "Mouse.h"
#include "Device.h"
#include "Roomba.h"

const double HEIGHT = 0.1;//0.4
const double ROTE_SPEED = PI / 40;
const int CAMERA_LENGTH = (int)( 20 * WORLD_SCALE );

AppCamera::AppCamera( RoombaPtr roomba ) :
_roomba( roomba ),
_mouse_x( 0 ),
Camera( roomba->getCentralPos( ) -  getCalcDir( roomba->getDir( ), HEIGHT ) * CAMERA_LENGTH, roomba->getCentralPos( ) ) {
	_dir = getCalcDir( roomba->getDir( ), HEIGHT );
}


AppCamera::~AppCamera( ) {

}

void AppCamera::move( ) {
	setTarget( _roomba->getCentralPos( ) );
	//‰ñ“]
	_dir = getCalcDir( _roomba->getDir( ), HEIGHT );
	//dir‚Éz‚ð‘«‚µ‚½‚Æ‚«‚É
	Vector pos = getTarget( ) - _dir.normalize( ) * CAMERA_LENGTH;
	setPos( pos );
}

void AppCamera::reset( ) {
	setPos( _roomba->getCentralPos( ) - getCalcDir( _roomba->getDir( ), HEIGHT ) * CAMERA_LENGTH );
	setTarget( _roomba->getCentralPos( ) );
	_dir = getCalcDir( _roomba->getDir( ), HEIGHT );
}


Vector AppCamera::getCalcDir( Vector dir, double HEIGHT ) const {
	dir.z = -HEIGHT;
	return dir.normalize( );
}
