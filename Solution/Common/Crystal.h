#pragma once
#include "Drawer.h"

PTR( Stage );

class Crystal {
public:
	Crystal( Vector pos );
	virtual ~Crystal( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	bool isHitting( Vector pos0, Vector pos1 );
	void damage( );
	bool isFinished( ) const;
	Vector getPos( ) const;
	void setVec( Vector vec );
private:
	Vector _pos;
	Vector _vec;
	bool _finished;
};

