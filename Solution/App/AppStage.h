#pragma once
#include "Stage.h"
#include <list>

PTR( Camera );
PTR( Delivery );
PTR( Crystal );
PTR( Viewer );

class AppStage : public Stage, public std::enable_shared_from_this< Stage > {
public:
	AppStage( int stage_num, ViewerPtr viewer );
	virtual ~AppStage( );
public:
	void update( CameraPtr camera );
	void draw( ) const;
	void reset( );
	Vector adjustCollisionToWall( Vector pos, Vector vec, const double radius );
	Vector adjustCollisionToCrystal( Vector pos, Vector vec, const double radius );
	CrystalPtr getHittingCrystal( Vector pos0, Vector pos1, Vector vec0, Vector vec1 ) const;
	bool isOnDelivery( Vector pos );
	int getDeliveryNum( ) const;
	int getDeliveryCount( ) const;
	std::list< CrystalPtr > getCrystalList( ) const;
private:
	void updateCrystal( );
	void updateDelivery( CameraPtr camera );
	void drawEarth( ) const;
	void drawWall( ) const;
	void drawCrystal( ) const;
	void drawDelivery( ) const;
	void drawCollisionLine( ) const;
	void loadCrystal( );
	void loadDelivery( );
	void loadMapData( );
	void loadPhase( );
	void load( int stage_num );
	Vector getAdjustPos( Vector pos, Vector base_pos ) const;
	bool isCollisionToSquare( Vector square_pos, Vector pos, double radius ) const;
	bool isCollisionToCircle( Vector circle_pos, Vector pos, double radius ) const;
	bool isCollisionToL( Vector pos_outside, Vector pos_inside, Vector pos, double radius ) const;
private:
	std::list< CrystalPtr > _crystals;
	std::list< DeliveryPtr > _deliverys;
	std::array< char, STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2 > _map_data;
	int _delivery_count;
	ViewerPtr _viewer;

};

