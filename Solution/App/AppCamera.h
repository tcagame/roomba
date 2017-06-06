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
	void shiftPos( Vector pos );
private:
	void move( );
	Vector getCalcDir( Vector dir ) const;
private:
	int _mouse_x;
	double _height;
	Vector _dir;
	Vector _before_target;
	RoombaPtr _roomba;
};

