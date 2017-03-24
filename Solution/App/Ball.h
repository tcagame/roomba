#pragma once
#include "Drawer.h"
#include "smart_ptr.h"
#include "Roomba.h"
PTR( Stage );

class Ball {
public:
	Ball( Vector pos, Roomba::BALL type );
	virtual ~Ball( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	Vector getPos( ) const;
	void addAccel( Vector vec );
	void move( Vector dir, Vector central_pos, Roomba::MOVE_STATE state, BallPtr target );
private:
	enum KEY {
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		MAX_KEY
	};
private:
	void moveTranslation( Vector dir, bool hold_key[ ] );
	void moveRotetionBoth( Vector dir, bool hold_key[ ] );
	void moveRotetionSide( Vector central_pos, bool hold_key[ ], BallPtr target );
	void deceleration( );
	void setAccel( Vector vec );
	void neutral( );
private:
	Roomba::BALL _type;
	Vector _pos;
	Vector _vec;
};

