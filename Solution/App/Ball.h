#pragma once
#include "Drawer.h"

class Ball {
public:
	Ball( Vector pos );
	virtual ~Ball( );
public:
	void update( );
	void draw( ) const;
	Vector getPos( ) const;
	void addAccel( Vector vec );
private:
	void move( );
	void deceleration( );
private:
	Vector _pos;
	Vector _vec;
};

