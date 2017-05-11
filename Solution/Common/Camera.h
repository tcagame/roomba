#pragma once

#include "mathmatics.h"

class Camera  {
public:
	Camera( Vector pos, Vector target );
	virtual ~Camera( );
public:
	void update( );
	Vector getPos( ) const;
	Vector getDir( ) const;
	Vector getTarget( ) const;
protected:
	virtual void move( );
	void setPos( Vector pos );
	void setTarget( Vector target );
	void setDir( Vector dir );
protected:
	Vector _pos;
	Vector _dir;
	Vector _target;
};