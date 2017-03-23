#pragma once
#include <vector>
#include <list>
#include "Drawer.h"

PTR( Crystal );

class Stage {
public:
	Stage( );
	virtual ~Stage( );
public:
	void draw( );
	CrystalPtr getHittingCrystal( Vector pos0, Vector pos1 );
private:
	static const int STAGE_SIZE = 110;
private:
	void drawCrystal( ) const;
	void drawWireFrame( );
private:
	std::vector< int > _map_data;
	std::list< CrystalPtr > _crystals;
	std::array< int, STAGE_SIZE > _stage_data;

};

