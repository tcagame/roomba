#include "EditorCamera.h"
#include "Keyboard.h"
#include "Mouse.h"

const Vector START_CAMERA_POS = Vector( 15, -15, 160 );
const Vector START_TARGET_POS = Vector( 15, 15, 0 );
const double MOVE_SPEED = 1.0;
const double ZOOM_SPEED = 9.0;
const double MAX_ZOOM = 30.0;

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
	if ( keyboard->isHoldKey( "W" ) ) {
		move.y += MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "S" ) ) {
		move.y -= MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "A" ) ) {
		move.x += MOVE_SPEED;
	}
	if ( keyboard->isHoldKey( "D" ) ) {
		move.x -= MOVE_SPEED;
	}

	MousePtr mouse = Mouse::getTask( );
	Vector pos = getPos( );
	Vector target = getTarget( );
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