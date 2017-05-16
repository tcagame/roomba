#pragma once
#include "Stage.h"
#include <list>

PTR( Crystal );

class AppStage : public Stage, public std::enable_shared_from_this< Stage > {
public:
	AppStage( int stage_num );
	virtual ~AppStage( );
public:
	void update( );
	void draw( ) const;
	void reset( );
	Vector adjustCollisionToWall( Vector pos, Vector vec, const double radius );
	Vector adjustCollisionToCrystal( Vector pos, Vector vec, const double radius );
	CrystalPtr getHittingCrystal( Vector pos0, Vector pos1 ) const;
	bool isOnStation( Vector pos, int type );
private:
	void updateCrystal( );
	void drawCrystal( ) const;
	void drawCollisionLine( ) const;
	void loadCrystal( );
	void loadMapData( );
	void load( int stage_num );
private:
	std::list< CrystalPtr > _crystals;
	std::array< char, STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2 > _map_data;
};

