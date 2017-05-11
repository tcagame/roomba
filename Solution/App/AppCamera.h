#pragma once
#include "Camera.h"
#include "smart_ptr.h"

PTR( Roomba );

class AppCamera : public Camera {
public:
	AppCamera( RoombaPtr roomba );
	virtual ~AppCamera( );
public:
	void reset( );
private:
	void move( );
	RoombaPtr _roomba;
private:
	int _mouse_x;
};

