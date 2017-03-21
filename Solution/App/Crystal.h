#pragma once
#include "Drawer.h"

class Crystal {
public:
	Crystal( Vector pos );
	virtual ~Crystal( );
public:
	void draw( );
private:
	Vector _pos;
};

