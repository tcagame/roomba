#pragma once
#include "Drawer.h"
#include "smart_ptr.h"
#include <array>

PTR( Ball );
PTR( Stage );

class Roomba {
public:
	Roomba( );
	virtual ~Roomba( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	Vector getPos( ) const;
private:
	enum BALL {
		BALL_LEFT,
		BALL_RIGHT,
		MAX_BALL
	};
	enum STATE {
		STATE_NEUTRAL,
		STATE_TRANSLATION,
		STATE_ROTETION,
		MAX_STATE,
	};
private:
	void deceleration( );
	void move( );
	void neutral( );
	void translation( );
	void rotetion( );
private:
	STATE _state;
	Vector _pos;
	Vector _vec;
	Vector _dir;
	double _range;
	bool _attack;
	double _rote_speed;
};

