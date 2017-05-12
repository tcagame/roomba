#pragma once
#include <vector>
#include <list>
#include "Drawer.h"

PTR( Crystal );
PTR( Timer );
PTR( Camera );
PTR( Roomba );

class Stage : public std::enable_shared_from_this< Stage > {
public:
	Stage( );
	virtual ~Stage( );
public:
	void update( RoombaPtr roomba );
	void draw( );
	Vector adjustCollisionToWall( Vector pos, Vector vec, const double radius );
	Vector adjustCollisionToCrystal( Vector pos, Vector vec, const double radius );
	bool isFinished( ) const;
	CrystalPtr getHittingCrystal( Vector pos0, Vector pos1 ) const;
	void reset( );
	void load( );
	void save( ) const;
	void setPhase( int phase );
	void drawEditor( ) const;
	void updateCursor( );
	void editWall( );
	void editCrystal( );
	void editStation( );
private:
	static const int STAGE_WIDTH_NUM = 40;
	static const int STAGE_HEIGHT_NUM = 35;
	static const int MAX_PHASE = 10;
	static const int MAX_STAGE = 4;
private:
	struct DATA {
		std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > wall;
		std::array< std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_PHASE > crystal;
		std::array< std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_PHASE > station;
	};
private:
	void updateCrystal( RoombaPtr roomba );
	void drawCrystal( ) const;
	void drawStation( ) const;
	void drawEarth( ) const;
	void drawWall( ) const;
	void drawBackground( ) const;
	void loadData( int stage_num );
	DATA getFileData( std::string filename ) const;
	void loadCrystal( );
	void loadEarth( );
	void loadWall( );
	void loadPhase( );
	void drawCollisionLine( ) const;
private:
	std::list< CrystalPtr > _crystals;
	std::vector< Drawer::ModelMDL > _walls;
	std::array< Drawer::ModelMDL, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _earth;
	std::array< char, STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2 > _map_data;
	DATA _data;
	int _phase;
	int _cursor_x;
	int _cursor_y;
	int _count;
	bool _finished;
};

