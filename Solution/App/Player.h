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
private:
	enum ROOMBA {
		ROOMBA_LEFT,
		ROOMBA_RIGHT,
		MAX_ROOMBA
	};
private:
	void updateRoomba( );
	void fall( );
private:
	std::array< RoombaPtr, MAX_ROOMBA > _roombas;
	Vector _pos;
	Vector _vec;
	Vector _dir;
	int _range;
};

