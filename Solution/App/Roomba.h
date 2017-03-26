#pragma once
#include "Drawer.h"
#include "smart_ptr.h"
#include <array>

PTR( Ball );
PTR( Stage );
PTR( Camera );
PTR( Timer );

class Roomba {
public:
	enum BALL {
		BALL_LEFT,
		BALL_RIGHT,
		MAX_BALL
	};
	enum MOVE_STATE {
		MOVE_STATE_NEUTRAL,
		MOVE_STATE_TRANSLATION,
		MOVE_STATE_ROTETION_SIDE,
		MOVE_STATE_ROTETION_BOTH,
		MAX_STATE,
	};
public:
	Roomba( );
	virtual ~Roomba( );
public:
	void update( StagePtr stage, CameraPtr camera, TimerPtr timer );
	void draw( ) const;
	Vector getCentralPos( ) const;
private:
	void move( StagePtr stage, CameraPtr camera );
	void centripetal( StagePtr stage );
	void updateState( );
	void attack( StagePtr stage, TimerPtr timer );
private:
	MOVE_STATE _state;
	int _neutral_count;
	std::array< BallPtr, MAX_BALL > _balls;
};

