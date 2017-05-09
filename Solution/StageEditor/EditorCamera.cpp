#include "EditorCamera.h"
#include "Keyboard.h"

const Vector START_CAMERA_POS = Vector( 0, -30, 160 );
const Vector START_TARGET_POS = Vector( 0, 0, 0 );
const double MOVE_SPEED = 1.0;

EditorCamera::EditorCamera( ) :
Camera( START_CAMERA_POS, START_TARGET_POS ) {

}


EditorCamera::~EditorCamera( ) {
}

void EditorCamera::move( ) {
	Vector move = Vector( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
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
	setPos( getPos( ) + move );
	setTarget( getTarget( ) + move );
}