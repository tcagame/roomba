#pragma once

#include "mathmatics.h"

class Camera  {
public:
	Camera( Vector pos, Vector target );
	virtual ~Camera( );
public:
	void update( );
	Vector getDir( ) const;
	Vector getTarget( ) const;
protected:
	virtual void move( );
	void setPos( Vector pos );
	void setTarget( Vector target );
	void setDir( Vector dir );
	Vector getPos( );
protected:
	Vector _pos;
	Vector _dir;
	Vector _target;
	int _mouse_x;
};