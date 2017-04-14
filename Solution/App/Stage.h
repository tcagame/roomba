#pragma once
#include <vector>
#include <list>
#include "Drawer.h"

PTR( Crystal );
PTR( Timer );

static const Vector CRYSTAL_SCALE( 2, 2, 2 );
static const Vector WALL_SCALE( 1, 1, 1 );

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
	static const int STAGE_WIDTH_NUM = 20;
	static const int STAGE_HEIGHT_NUM = 20;
	static const int MAX_WAVE = 3;
private:
	void updateCrystal( );
	void updateTimer( );
	void drawCrystal( ) const;
	void drawEarth( ) const;
	void drawWall( ) const;
	void drawBackground( ) const;
	void loadCrystalData( );
	void loadWallModel( );
private:
	std::list< CrystalPtr > _crystals;
	std::array< int, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _stage_data;
	std::array< std::array< int, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_WAVE > _waves;
	int _wave;
	bool _finished;
	ModelPtr _model[ STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ];
};

