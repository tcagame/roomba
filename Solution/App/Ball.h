#pragma once
#include "Drawer.h"
#include "smart_ptr.h"

PTR( Stage );

class Ball {
public:
	Ball( Vector pos );
	virtual ~Ball( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	Vector getPos( ) const;
	void addAccel( Vector vec );
private:
	void move( StagePtr stage );
	void deceleration( );
private:
	Vector _pos;
	Vector _vec;
};

