#include "AppCamera.h"
#include "define.h"
#include "Mouse.h"
#include "Device.h"
#include "Roomba.h"

const double HEIGHT_RATIO = 0.7;
const double HEIGHT_SPEED = 0.004;
const double MAX_HEIGHT = 0.8;//íxÇ¢Ç∆Ç´ÇÃçÇÇ≥
const double MIN_HEIGHT = 0.3;//ëÅÇ¢Ç∆Ç´ÇÃçÇÇ≥
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
	Vector target = _roomba->getCentralPos( );
	setTarget( target );
	//çÇÇ≥åvéZ
	Vector vec = target - _before_target;
	double height = MAX_HEIGHT - vec.getLength( ) * HEIGHT_RATIO;
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
	//âÒì]
	Vector dir = target - getPos( );
	dir.z = 0;
	Matrix rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), _roomba->getRotSpeed( ) * ROTE_SPEED );
	dir = rot.multiply( dir );
	//ç¿ïWåvéZ
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