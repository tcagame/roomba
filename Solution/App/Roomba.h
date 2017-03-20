#pragma once
#include "Drawer.h"

class Roomba {
public:
	Roomba( bool left );
	virtual ~Roomba( );
public:
	void update( Vector pos, Vector dir );
	void draw( ) const;
	bool isLeft( ) const;
	Vector getPos( ) const;
private:
	bool _left;
	Vector _pos;
	Vector _dir;
};

