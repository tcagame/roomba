#pragma once

#include "mathmatics.h"
#include "Task.h"
#include <string>

PTR( Camera );
PTR( Roomba );

class Camera  {
public:
	Camera( );
	virtual ~Camera( );
public:
	void update( RoombaPtr Roomba );
	void move( );
	Vector getDir( ) const;
protected:
	Vector _pos;
	Vector _dir;
	Vector _target;
	int _mouse_x;
};