#pragma once
#include <vector>
#include <list>
#include "Drawer.h"

PTR( Crystal );
PTR( Timer );


class Stage {
public:
	Stage( );
	virtual ~Stage( );
public:
	void update( );
	void draw( );
	bool isCollisionWall( Vector pos );
	bool isFinished( ) const;
	CrystalPtr getHittingCrystal( Vector pos0, Vector pos1 );
private:
	static const int STAGE_WIDTH_NUM = 10;
	static const int STAGE_HEIGHT_NUM = 10;
	static const int MAX_WAVE = 3;
private:
	void updateCrystal( );
	void updateTimer( );
	void drawCrystal( ) const;
	void drawWireFrame( );
	void loadCrystalData( );
private:
	std::list< CrystalPtr > _crystals;
	std::array< int, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _stage_data;
	std::array< std::array< int, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_WAVE > _waves;
	int _wave;
	bool _finished;
};

