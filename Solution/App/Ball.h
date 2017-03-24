#pragma once
#include "Drawer.h"

class Ball {
public:
	Ball( Vector pos );
	virtual ~Ball( );
public:
	void update( Vector pos );
	void draw( ) const;
	Vector getPos( ) const;
private:
	Vector _pos;
};

