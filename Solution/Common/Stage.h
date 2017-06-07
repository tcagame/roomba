#pragma once
#include <vector>
#include <array>
#include "Drawer.h"
#include "define.h"

PTR( Stage );
PTR( Camera );

class Stage {
public:
	struct DATA {
		char wall;
		char crystal;
		char delivery;
	};
public:
	Stage( );
	virtual ~Stage( );
public:
	virtual void update( CameraPtr camera ) = 0;
	virtual void draw( ) const = 0;
	virtual void reset( );
	int getMaxDeliveryNum( ) const;
	const DATA& getData( int idx ) const;
	const std::array< DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >& getData( ) const;
protected:
	static const int MAX_STAGE = 4;
	static const int MAX_LINK = 5;
protected:
	void debug( );
	void setData( DATA& data, int idx );
	void setData( std::array< DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >& data );
	void drawModel( ) const;
	void load( int stage_num );
	void loadData( std::string filename );
	void saveData( std::string filename ) const;
	virtual void drawCrystal( ) const;
	virtual void drawDelivery( ) const;
	void loadWall( );
	virtual void loadCrystal( );
	virtual void loadDelivery( );
	const std::vector< Drawer::ModelMDL >& getWalls( ) const;
private:
	virtual void drawEarth( ) const = 0;
	virtual void drawWall( ) const = 0;
private:
	std::array< DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _data;
	std::vector< Drawer::ModelMDL > _walls;
	std::array< Drawer::ModelMDL, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _earth;
	int _max_delivery;
};

