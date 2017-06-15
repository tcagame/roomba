#pragma once
#include "Stage.h"
#include <list>

PTR( Camera );
PTR( Delivery );
PTR( Crystal );
PTR( Viewer );
PTR( Timer );
PTR( Roomba );

class AppStage : public Stage, public std::enable_shared_from_this< Stage > {
public:
	AppStage( int stage_num, ViewerPtr viewer, TimerPtr timer, RoombaPtr roomba );
	virtual ~AppStage( );
public:
	void update( CameraPtr camera );
	void draw( ) const;
	void reset( );
	bool isFinished( ) const;
	Vector adjustCollisionToWall( Vector pos, Vector vec, const double radius );
	Vector adjustCollisionToCrystal( Vector pos, Vector& vec, const double radius );
	CrystalPtr getHittingCrystal( Vector& pos0, Vector& pos1, Vector& vec0, Vector& vec1) const;
	bool isOnDelivery( Vector& pos );
	int getDeliveryCount( ) const;
	std::list< CrystalPtr > getCrystalList( ) const;
	void eraseOldDelivery( );
private:
	void updateCrystal( TimerPtr timer );
	void updateDelivery( CameraPtr camera );
	void drawEarth( ) const;
	void drawWall( ) const;
	void drawCrystal( ) const;
	void drawDelivery( ) const;
	void loadCrystal( );
	void loadDelivery( );
	void loadMapData( );
	void adjustPos( Vector& pos, Vector& base_pos ) const;
	bool isCollisionToSquare( Vector& square_pos, Vector pos, double radius ) const;
	bool isCollisionToCircle( Vector circle_pos, Vector pos, double radius ) const;
	bool isCollisionToL( Vector pos_outside, Vector pos_inside, Vector pos, double radius ) const;
	void collideCrystal( CrystalPtr crystal );
private:
	std::list< CrystalPtr > _crystals;
	std::list< DeliveryPtr > _deliverys;
	std::array< char, STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2 > _map_data;
	int _delivery_count;
	bool _finished;
	ViewerPtr _viewer;
	TimerPtr _timer;
	RoombaPtr _roomba;
};

