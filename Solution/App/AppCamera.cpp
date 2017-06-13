#include "AppCamera.h"
#include "define.h"
#include "Device.h"
#include "Roomba.h"

const double HEIGHT_RATIO = 2.0;
const double HEIGHT_SPEED = 0.008;
const double MAX_HEIGHT = 1.3;//’x‚¢‚Æ‚«‚Ì‚‚³
const double MIN_HEIGHT = 0.3;//‘‚¢‚Æ‚«‚Ì‚‚³
const double ROTE_SPEED = 0.05;
const int CAMERA_LENGTH = (int)( 40 * WORLD_SCALE );

AppCamera::AppCamera( RoombaPtr roomba ) :
_roomba( roomba ),
_mouse_x( 0 ),
_height( MAX_HEIGHT ),
Camera( roomba->getCentralPos( ) -  getCalcDir( roomba->getDir( ) ) * CAMERA_LENGTH, roomba->getCentralPos( ) ) {
	_dir = getCalcDir( roomba->getDir( ) );
	_before_target = roomba->getCentralPos( );
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
	Vector target = _roomba->getCentralPos( );
	Vector dir = target - getPos( );
	setTarget( target );
	//‚‚³ŒvŽZ
	Vector vec = target - _before_target;
	double angle = Vector( 0, 0, dir.z ).angle( Vector( vec.x, vec.y ) );
	double length = vec.getLength( ) * HEIGHT_RATIO;
	if ( angle > PI / 3 ) {
		length = 0;
	}
	double height = MAX_HEIGHT - length;
	if ( fabs( _height - height ) < HEIGHT_SPEED ) {
		_height = height;
	} else if ( _height > height ) {
		_height -= HEIGHT_SPEED;
	} else {
		_height += HEIGHT_SPEED;
	}

	if ( _height < MIN_HEIGHT ) {
		_height = MIN_HEIGHT;
	}
	_before_target = target;
	//‰ñ“]
	dir.z = 0;
	Matrix rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), rot_speed );
	dir = rot.multiply( dir );
	//À•WŒvŽZ
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
	_before_target = pos;
	setTarget( pos );
	setPos( pos - _dir * CAMERA_LENGTH );
}

Vector AppCamera::getCalcDir( Vector dir ) const {
	dir = dir.normalize( );
	dir.z = -_height;
	return dir.normalize( );
}