#include "AppCamera.h"
#include "define.h"
#include "Mouse.h"
#include "Device.h"
#include "Roomba.h"

const double HEIGHT = 0.4;//0.4
const double ROTE_SPEED = 0.05;
const int CAMERA_LENGTH = (int)( 40 * WORLD_SCALE );

AppCamera::AppCamera( RoombaPtr roomba ) :
_roomba( roomba ),
_mouse_x( 0 ),
Camera( roomba->getCentralPos( ) -  getCalcDir( roomba->getDir( ) ) * CAMERA_LENGTH, roomba->getCentralPos( ) ) {
	_dir = getCalcDir( roomba->getDir( ) );
}


AppCamera::~AppCamera( ) {

}

void AppCamera::move( ) {
	Vector target = _roomba->getCentralPos( );
	setTarget( target );
	//回転カメラワーク
	Vector dir = target - getPos( );
	dir.z = 0;
	Matrix rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), _roomba->getRotSpeed( ) * ROTE_SPEED );
	dir = rot.multiply( dir );
	//座標計算
	_dir = getCalcDir( dir );
	Vector pos = target - _dir * CAMERA_LENGTH;
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