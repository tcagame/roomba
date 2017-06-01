#pragma once

#include "mathmatics.h"

class Camera  {
public:
	Camera( Vector pos, Vector target );
	virtual ~Camera( );
public:
	void update( );
	Vector getPos( ) const;
	Vector getTarget( ) const;
	Vector getDir( ) const;
protected:
	virtual void move( );
	void setPos( Vector pos );
	void setTarget( Vector target );
private:
	Vector _pos;
	Vector _target;
};