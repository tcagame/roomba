#pragma once
#include "Drawer.h"
#include "smart_ptr.h"
#include <array>

PTR( Ball );
PTR( Stage );
PTR( Camera );
PTR( Timer );

class Roomba {
public:
	enum MOVE_STATE {
		MOVE_STATE_TRANSLATION,
		MOVE_STATE_ROTATION_SIDE,
		MOVE_STATE_ROTATION_BOTH,
		MAX_STATE,
	};
public:
	Roomba( );
	virtual ~Roomba( );
public:
	void update( StagePtr stage, CameraPtr camera, TimerPtr timer );
	void draw( ) const;
	void reset( );
	Vector getCentralPos( ) const;
private:
	void move( StagePtr stage, CameraPtr camera );
	void updateState( CameraPtr camera );
	void attack( StagePtr stage, TimerPtr timer );
	void moveTranslation( const Vector& camera_dir, const Vector& right, const Vector& left );
	void moveRotetionBoth( );
	void moveRotetionSide( const Vector& camera_dir, Vector right, Vector left );
	void checkLeftRight( CameraPtr camera );
private:
	MOVE_STATE _state;
	std::array< BallPtr, 2 > _balls;
};

