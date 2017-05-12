#pragma once
#include "Drawer.h"

class Crystal {
public:
	Crystal( Vector pos );
	virtual ~Crystal( );
public:
	void draw( );
	bool isHitting( Vector pos0, Vector pos1 );
	void damage( );
	bool isFinished( ) const;
	Vector getPos( ) const;
	void setPos( Vector pos );
private:
	Vector _pos;
	bool _finished;
};

