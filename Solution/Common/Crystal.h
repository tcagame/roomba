#pragma once
#include "Drawer.h"

PTR( Stage );
PTR( Roomba );

class Crystal {
public:
	Crystal( Vector pos );
	virtual ~Crystal( );
public:
	void update( StagePtr stage, RoombaPtr roomba );
	void draw( ) const;
	bool isHitting( Vector pos0, Vector pos1 );
	Vector adjustHitToRoomba( Vector pos, Vector vec, double radius );
	void damage( );
	bool isFinished( ) const;
	Vector getPos( ) const;
	void setVec( Vector vec );
	bool isDropDown( ) const;
private:
	Vector _pos;
	Vector _vec;
	bool _finished;
	bool _drop_down;
};

