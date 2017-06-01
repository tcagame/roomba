#include "EditorCamera.h"
#include "Keyboard.h"
#include "Mouse.h"

const Vector START_CAMERA_POS = Vector( 15, -15, 100 );
const Vector START_TARGET_POS = Vector( 15, 15, 0 );
const double MOVE_SPEED = 1.0;
const double ZOOM_SPEED = 9.0;
const double MAX_ZOOM = 30.0;
const double ROT_SPEED = PI / 60;

EditorCamera::EditorCamera( ) :
Camera( START_CAMERA_POS, START_TARGET_POS ) {
	MousePtr mouse = Mouse::getTask( );
	_before_mouse_pos = mouse->getPos( );
}


EditorCamera::~EditorCamera( ) {
}

void EditorCamera::move( ) {
	Vector move = Vector( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		move.y += MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		move.y -= MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		move.x += MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
		move.x -= MOVE_SPEED;
	}
	int axis_z = 0;//‰¡•ûŒü‰ñ“]
	if ( keyboard->isHoldKey( "D" ) ) {
		axis_z = 1;
	}
	if ( keyboard->isHoldKey( "A" ) ) {
		axis_z = -1;
	}
	int axis_x = 0;//c•ûŒü‰ñ“]
	if ( keyboard->isHoldKey( "W" ) ) {
		axis_x = -1;
	}
	if ( keyboard->isHoldKey( "S" ) ) {
		axis_x = 1;
	}

	MousePtr mouse = Mouse::getTask( );
	Vector pos = getPos( );
	Vector target = getTarget( );
	if ( axis_z != 0 || axis_x != 0 ) {
		double length = ( target - pos ).getLength( );
		Vector dir = ( target - pos ).normalize( );
		Matrix mat = Matrix::makeTransformRotation( Vector( axis_x, 0, axis_z ), ROT_SPEED );
		dir = mat.multiply( dir ).normalize( );
		dir.z += axis_x * ROT_SPEED * 2;
		if ( dir.z < -0.5 ) {
			dir.z = -0.5;
		}
		if ( dir.z > -0.1 ) {
			dir.z = -0.1;
		}
		pos = target - dir.normalize( ) * length;
	}

	int wheel = mouse->getWheelRotValue( );
	if ( wheel > 0 ) {
		do {
			Vector distance = target - pos;
			if ( distance.getLength( ) < MAX_ZOOM ) {
				break;
			}
			pos += distance.normalize( ) * ZOOM_SPEED;
		} while ( 0 );
	}
	if ( wheel < 0 ) {
		pos += ( target - pos ).normalize( ) * -ZOOM_SPEED;
	}
	setPos( pos + move );
	setTarget( target + move );
}