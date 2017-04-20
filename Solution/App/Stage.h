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
	Vector getCollisionWall( Vector pos, Vector vec, const double radius );
	bool isFinished( ) const;
	CrystalPtr getHittingCrystal( Vector pos0, Vector pos1 );
	void reset( );
private:
	static const int STAGE_WIDTH_NUM = 40;
	static const int STAGE_HEIGHT_NUM = 35;
	static const int MAX_WAVE = 1;
private:
	void updateCrystal( );
	void drawCrystal( ) const;
	void drawEarth( ) const;
	void drawWall( ) const;
	void drawBackground( ) const;
	void loadCrystalData( );
	void loadModel( );
	void drawCollisionLine( ) const;
private:
	std::list< CrystalPtr > _crystals;
	std::array< int, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _stage_data;
	std::array< std::array< int, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_WAVE > _waves;
	int _wave;
	bool _finished;
	std::array< Drawer::ModelMDL, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _earth;
	std::vector< Drawer::ModelMDL > _walls;
	std::array< char, STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2 > _map_data;
};

