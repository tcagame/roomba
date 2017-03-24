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
	Vector getCentralPos( ) const;
private:
	enum STATE {
		STATE_NEUTRAL,
		STATE_TRANSLATION,
		STATE_ROTETION_SIDE,
		STATE_ROTETION_BOTH,
		MAX_STATE,
	};
	enum BALL {
		BALL_LEFT,
		BALL_RIGHT,
		MAX_BALL
	};
private:
	void move( );
	void centripetal( );
	void decelerationRotetion( );
	void neutral( );
	void translation( );
	void attack( StagePtr stage );
	bool isCollision( StagePtr stage );
private:
	STATE _state;
	Vector _dir;
	bool _attacking;
	double _rote_speed;
	std::array< BallPtr, MAX_BALL > _balls;
};

