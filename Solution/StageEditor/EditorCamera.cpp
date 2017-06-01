#include "EditorCamera.h"
#include "Keyboard.h"
#include "Mouse.h"

const Vector START_CAMERA_POS = Vector( 15, -15, 100 );
const Vector START_TARGET_POS = Vector( 15, 15, 0 );
const double MOVE_SPEED = 1.0;
const double ZOOM_SPEED = 9.0;
const int MAX_LENGTH = 150;
const int MIN_LENGTH = 30;
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
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		axis_x = -1;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		axis_x = 1;
	}
	int axis_z = 0;//‰¡•ûŒü‰ñ“]
	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		axis_z = -1;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		axis_z = 1;
	}

	MousePtr mouse = Mouse::getTask( );
	Vector dir = getDir( );
	if ( axis_z != 0 || axis_x != 0 ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( axis_x, 0, axis_z ), ROT_SPEED );
		dir = mat.multiply( dir ).normalize( );
		dir.z += axis_x * ROT_SPEED * 2;
		if ( dir.z < -0.5 ) {
			dir.z = -0.5;
		}
		if ( dir.z > -0.1 ) {
			dir.z = -0.1;
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