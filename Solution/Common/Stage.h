#pragma once
#include <vector>
#include <array>
#include "Drawer.h"

PTR( Stage );

class Stage {
public:
	Stage( );
	virtual ~Stage( );
public:
	virtual void update( ) = 0;
	virtual void draw( ) const = 0;
	bool isFinished( ) const;
	virtual void reset( );
protected:
	static const int STAGE_WIDTH_NUM = 40;
	static const int STAGE_HEIGHT_NUM = 35;
	static const int MAX_PHASE = 10;
	static const int MAX_STAGE = 4;
	static const int MAX_LINK = 5;
protected:
	struct DATA {
		std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > wall;
		std::array< std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_PHASE > crystal;
		std::array< std::array< char, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >, MAX_PHASE > station;
	};
protected:
	void debug( );
	void setData( DATA data );
	DATA getData( ) const;
	void setPhase( int phase );
	void drawModel( ) const;
protected:
	void load( int stage_num );
	virtual void drawCrystal( ) const;
	void loadEarth( );
	void loadWall( );
	void loadPhase( );
	virtual void loadCrystal( );
	int getPhase( ) const;
private:
	void drawBackground( ) const;
	void drawEarth( ) const;
	void drawWall( ) const;
	void drawStation( ) const;
private:
	std::vector< Drawer::ModelMDL > _walls;
	std::array< Drawer::ModelMDL, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _earth;
	DATA _data;
	int _phase;
	bool _finished;
};

