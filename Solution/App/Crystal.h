#pragma once
#include "Drawer.h"

class Crystal {
public:
	Crystal( Vector pos );
	virtual ~Crystal( );
public:
	void draw( );
	bool isHitting( Vector pos0, Vector pos1 );
private:
	Vector _pos;
};

