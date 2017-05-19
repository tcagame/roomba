#pragma once
#include <vector>
#include <array>
#include "Drawer.h"
#include "define.h"

PTR( Stage );

class Stage {
public:
	struct DATA {
		std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > wall;
		std::array< std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_PHASE > crystal;
		std::array< std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_PHASE > station;
	};
public:
	Stage( );
	virtual ~Stage( );
public:
	virtual void update( ) = 0;
	virtual void draw( ) const = 0;
	bool isFinished( ) const;
	virtual void reset( );
	int getMaxStationNum( ) const;
	int getPhase( ) const;
	DATA getData( ) const;
protected:
	static const int MAX_STAGE = 4;
	static const int MAX_LINK = 5;
protected:
	void debug( );
	void setData( DATA data );
	void setPhase( int phase );
	void drawModel( ) const;
	void load( int stage_num );
	virtual void drawCrystal( ) const;
	void loadWall( );
	void loadPhase( );
	virtual void loadCrystal( );
	std::vector< Drawer::ModelMDL > getWalls( ) const;
private:
	virtual void drawEarth( ) const = 0;
	virtual void drawWall( ) const = 0;
	virtual void drawStation( ) const = 0;
private:
	std::vector< Drawer::ModelMDL > _walls;
	std::array< Drawer::ModelMDL, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _earth;
	DATA _data;
	int _phase;
	bool _finished;
	int _max_station;
};

