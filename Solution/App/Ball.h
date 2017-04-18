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
private:
	enum KEY {
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		MAX_KEY
	};
private:
	void moveTranslation( Vector camera_dir, int dir_x, int dir_y );
	void moveRotetionBoth( Vector other_pos, bool hold_key[ ], bool left );
	void moveRotetionSide( bool hold_key[ ], BallPtr target );
	void deceleration( );
	void neutral( Vector dir, Vector other_pos );
private:
	Vector _pos;
	Vector _vec;
	bool _left;
};

