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
	Vector getPos( ) const;
	Vector getVec( ) const;
	void setAccel( Vector vec );
	bool isAttacking( ) const;
	void move( Vector camera_dir, Roomba::MOVE_STATE state, BallPtr target );
	void reset( Vector pos );
	void checkLeft( Vector camera_dir, Vector other_pos );
private:
	enum KEY {
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		MAX_KEY
	};
private:
	void moveTranslation(  BallPtr target, Vector device_dir, Vector camera_dir );
	void moveRotetionBoth( BallPtr target, Vector device_dir );
	void moveRotetionSide( BallPtr target, Vector device_dir );
	void deceleration( );
private:
	Vector _pos;
	Vector _vec;
	bool _left;
	bool _axis;
};

