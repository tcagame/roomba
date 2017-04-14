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
	void reset( );
private:
	static const int STAGE_WIDTH_NUM = 50;
	static const int STAGE_HEIGHT_NUM = 50;
	static const int MAX_WAVE = 3;
private:
	enum WALL {
		WALL_0,
		WALL_1,
		WALL_2,
		MAX_WALL
	};
private:
	void updateCrystal( );
	void drawCrystal( ) const;
	void drawEarth( ) const;
	void drawWall( ) const;
	void drawBackground( ) const;
	void loadCrystalData( );
	void loadModel( );
private:
	std::list< CrystalPtr > _crystals;
	std::array< int, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _stage_data;
	std::array< std::array< int, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_WAVE > _waves;
	int _wave;
	bool _finished;
	ModelPtr _wall;
	ModelPtr _earth;
};

