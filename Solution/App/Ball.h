#pragma once
#include "Drawer.h"
#include "Roomba.h"

PTR( Stage );

class Ball {
public:
	Ball( Vector pos );
	virtual ~Ball( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	Vector getPos( ) const;
	Vector getVec( ) const;
	void addAccel( Vector vec );
	bool isAttacking( ) const;
	void move( Vector dir, Roomba::MOVE_STATE state, BallPtr target );
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
	void moveRotetionBoth( Vector other_pos, bool hold_key[ ], bool left );
	void moveRotetionSide( bool hold_key[ ], BallPtr target );
	void deceleration( );
	void setAccel( Vector vec );
	void neutral( Vector dir, Vector other_pos );
private:
	Vector _pos;
	Vector _vec;
	bool _left;
};

