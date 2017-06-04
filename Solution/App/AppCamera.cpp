#include "AppCamera.h"
#include "define.h"
#include "Mouse.h"
#include "Device.h"
#include "Roomba.h"

const double HEIGHT = 0.4;//0.4
const double ROTE_SPEED = PI / 40;
const int CAMERA_LENGTH = (int)( 40 * WORLD_SCALE );
const double MOVE_RATIO = 0.00001;

AppCamera::AppCamera( RoombaPtr roomba ) :
_roomba( roomba ),
_mouse_x( 0 ),
Camera( roomba->getCentralPos( ) -  getCalcDir( roomba->getDir( ) ) * CAMERA_LENGTH, roomba->getCentralPos( ) ) {
	_dir = getCalcDir( roomba->getDir( ) );
	_before_roomba_pos = roomba->getCentralPos( );
}


AppCamera::~AppCamera( ) {

}

void AppCamera::move( ) {
	Vector target = _roomba->getCentralPos( );
	setTarget( target );
	//回転カメラワーク
	Vector roomba_pos = _roomba->getCentralPos( );
	Vector dir = getCalcDir( roomba_pos - _before_roomba_pos );
	_before_roomba_pos = roomba_pos;
	Vector pos_target = getTarget( ) - dir.normalize( ) * CAMERA_LENGTH;
	Vector pos = getPos( );
	pos += ( pos_target - pos ) * MOVE_RATIO;
	_dir = getCalcDir( ( target - pos ) );
	//座標計算
	pos = target - _dir * CAMERA_LENGTH;
	setPos( pos );
}

void AppCamera::reset( ) {
	setPos( _roomba->getCentralPos( ) - getCalcDir( _roomba->getDir( ) ) * CAMERA_LENGTH );
	setTarget( _roomba->getCentralPos( ) );
	_dir = getCalcDir( _roomba->getDir( ) );
}

void AppCamera::shiftPos( Vector pos ) {
	setTarget( pos );
	setPos( pos - _dir * CAMERA_LENGTH );
}

Vector AppCamera::getCalcDir( Vector dir ) const {
	dir = dir.normalize( );
	dir.z = -HEIGHT;
	return dir.normalize( );
}
