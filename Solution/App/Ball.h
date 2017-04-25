#pragma once
#include "Drawer.h"
#include "Roomba.h"
#include "define.h"

PTR( Stage );
static const double BALL_RADIUS = WORLD_SCALE;

class Ball {
public:
	Ball( Vector pos );
	virtual ~Ball( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	void reset( Vector pos );
	void setAccel( const Vector& vec );
	void deceleration( const double accel );
	Vector getPos( ) const;
	Vector getVec( ) const;
private:
	Vector _pos;
	Vector _vec;
};

