#include "EditorCamera.h"
#include "Keyboard.h"
#include "Mouse.h"

const Vector START_CAMERA_POS = Vector( 15, -15, 15 );
const Vector START_TARGET_POS = Vector( 15, 15, 0 );
const double MOVE_SPEED = 1.0;
const double ZOOM_SPEED = 9.0;
const int MAX_LENGTH = 400;
const int MIN_LENGTH = 30;
const double MIN_HEIGHT = 0.01;
const double MAX_HEIGHT = 1.0;
const double ROT_SPEED = PI / 60;

EditorCamera::EditorCamera( ) :
Camera( START_CAMERA_POS, START_TARGET_POS ) {
	MousePtr mouse = Mouse::getTask( );
	_before_mouse_pos = mouse->getPos( );
	_length = MAX_LENGTH;
}


EditorCamera::~EditorCamera( ) {
}

void EditorCamera::move( ) {
	Vector move = Vector( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "W" ) ) {
		move.y -= MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "S" ) ) {
		move.y += MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "A" ) ) {
		move.x -= MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "D" ) ) {
		move.x += MOVE_SPEED;
	}
	int axis_x = 0;//c•ûŒü‰ñ“]
	if ( keyboard->isHoldKey( "NUM8" ) ) {
		axis_x = -1;
	}
	if ( keyboard->isHoldKey( "NUM2" ) ) {
		axis_x = 1;
	}
	int axis_z = 0;//‰¡•ûŒü‰ñ“]
	if ( keyboard->isHoldKey( "NUM4" ) ) {
		axis_z = -1;
	}
	if ( keyboard->isHoldKey( "NUM6" ) ) {
		axis_z = 1;
	}

	MousePtr mouse = Mouse::getTask( );
	Vector dir = getDir( );
	if ( axis_z != 0 || axis_x != 0 ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, axis_z ), ROT_SPEED );
		dir = mat.multiply( dir ).normalize( );
		dir.z += axis_x * ROT_SPEED * 2;
		if ( dir.z < -MAX_HEIGHT ) {
			dir.z = -MAX_HEIGHT;
		}
		if ( dir.z > -MIN_HEIGHT ) {
			dir.z = -MIN_HEIGHT;
		}
	}

	int wheel = mouse->getWheelRotValue( );
	_length -= wheel * ZOOM_SPEED;

	if ( _length > MAX_LENGTH ) {
		_length = MAX_LENGTH;
	}
	if ( _length < MIN_LENGTH ) {
		_length = MIN_LENGTH;
	}
	Vector tmp_dir = dir;
	tmp_dir.z = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( tmp_dir ) * -1, Vector( 0, -1 ).angle( tmp_dir ) );
	move = mat.multiply( move );
	Vector target = getTarget( ) + move;
	Vector pos = target - dir.normalize( ) * _length;
	setPos( pos );
	setTarget( target );
}