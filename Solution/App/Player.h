#pragma once
#include "Drawer.h"
#include "smart_ptr.h"
#include <array>

PTR( Roomba );
PTR( Stage );

class Player {
public:
	Player( );
	virtual ~Player( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	Vector getPos( ) const;
private:
	enum ROOMBA {
		ROOMBA_LEFT,
		ROOMBA_RIGHT,
		MAX_ROOMBA
	};
	enum STATE {
		STATE_NEUTRAL,
		STATE_TRANSLATION,
		STATE_ROTETION,
		MAX_STATE,
	};
private:
	void updateRoomba( );
	void deceleration( );
	void fall( );
	void move( );
	void neutral( );
	void translation( );
	void rotetion( );
	bool isCollision( StagePtr stage );
private:
	std::array< RoombaPtr, MAX_ROOMBA > _roombas;
	STATE _state;
	Vector _pos;
	Vector _vec;
	Vector _dir;
	double _range;
	bool _attack;
	double _rote_speed;
};

