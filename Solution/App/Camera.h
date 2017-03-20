#pragma once

#include "mathmatics.h"
#include "Task.h"
#include <string>

PTR( Camera );

class Camera  {
public:
	Camera( );
	virtual ~Camera( );
public:
	void update( );
	void move( );
protected:
	Vector _pos;
	Vector _target;
	Vector _before_mouse_pos;
};