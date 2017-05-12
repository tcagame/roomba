#pragma once
#include "Drawer.h"
#include "Roomba.h"
#include "define.h"

PTR( Stage );
static const double BALL_RADIUS = WORLD_SCALE / 2;

class Ball {
public:
	Ball( const Vector& pos );
	virtual ~Ball( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	void reset( const Vector& pos );
	void addForce( const Vector& force );
	void deceleration( const double accel );
	Vector getPos( ) const;
	Vector getVec( ) const;
private:
	Vector _pos;
	Vector _vec;
};

