#include "AppCamera.h"
#include "define.h"
#include "Device.h"
#include "Roomba.h"

const double HEIGHT_RATIO = 0.06;
const double HEIGHT_SPEED = 0.01;
const double MAX_HEIGHT = 1.3;//’x‚¢‚Æ‚«‚Ì‚‚³
const double MIN_HEIGHT = 0.5;//‘‚¢‚Æ‚«‚Ì‚‚³
const double ROTE_SPEED = 0.05;
const int CAMERA_LENGTH = (int)( 40 * WORLD_SCALE );

AppCamera::AppCamera( RoombaPtr roomba ) :
_roomba( roomba ),
_mouse_x( 0 ),
_height( MAX_HEIGHT ),
Camera( Vector( roomba->getStartPos( ).x, roomba->getStartPos( ).y ) -  getCalcDir( Vector( 0, 1 ) ) * CAMERA_LENGTH, Vector( roomba->getStartPos( ).x, roomba->getStartPos( ).y )  ) {
	_before_target = roomba->getStartPos( );
	_before_target.z = 0;
}


AppCamera::~AppCamera( ) {

}

void AppCamera::move( ) {
	double rot_speed = 0;
	if ( _roomba->isWait( ) ) {
		DevicePtr device = Device::getTask( );
		double right_stick = device->getRightDirY( );
		double left_stick = device->getDirY( );
		if ( right_stick > 0 && left_stick < 0 ) {
			rot_speed = -ROTE_SPEED;
		}
		if ( right_stick < 0 && left_stick > 0 ) {
			rot_speed = ROTE_SPEED;
		}
	} else {
		rot_speed = _roomba->getRotSpeed( ) * ROTE_SPEED;
	}
	Vector target;
	if ( _roomba->isStarting( ) ) {
		target = _roomba->getStartPos( );
	} else {
		target = _roomba->getCentralPos( );
	}
	target.z = 0;
	Vector dir = target - getPos( );
	setTarget( target );
	//‚‚³ŒvŽZ
	Vector vec = target - _before_target;
	double ratio = 0;
	if ( vec.getLength( ) > 0.001 ) {
		ratio = vec.getLength( ) / _roomba->getMaxSpeed( );
	}
	double height_vec = ( MAX_HEIGHT - ( MAX_HEIGHT - MIN_HEIGHT ) * ratio ) - _height;
	height_vec *= HEIGHT_RATIO;
	if ( fabs( height_vec ) > HEIGHT_SPEED ) {
		height_vec = HEIGHT_SPEED * ( 1 - ( height_vec < 0 ) * 2 );
	}
	_height += height_vec;
	_before_target = target;
	//‰ñ“]
	dir.z = 0;
	Matrix rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), rot_speed );
	dir = rot.multiply( dir );
	//À•WŒvŽZ
	Vector pos = target - getCalcDir( dir ) * CAMERA_LENGTH;
	setPos( pos );
}

void AppCamera::reset( ) {
	setPos( _roomba->getCentralPos( ) - getCalcDir( _roomba->getDir( ) ) * CAMERA_LENGTH );
	setTarget( _roomba->getCentralPos( ) );
}

void AppCamera::shiftPos( Vector pos ) {
	_before_target = pos;
	Vector dir = getDir( );
	setTarget( pos );
	setPos( pos - dir * CAMERA_LENGTH );
}

Vector AppCamera::getCalcDir( Vector dir ) const {
	dir = dir.normalize( );
	dir.z = -_height;
	return dir.normalize( );
}