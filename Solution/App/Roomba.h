#pragma once
#include "Drawer.h"
#include <array>

PTR( Stage );
PTR( Camera );
PTR( Ball );
PTR( Crystal );

class Roomba {
public:
	enum MOVE_STATE {
		MOVE_STATE_NEUTRAL,
		MOVE_STATE_TRANSLATION,
		MOVE_STATE_ROTATION,
		MAX_STATE,
	};
public:
	Roomba( );
	virtual ~Roomba( );
public:
	void update( StagePtr stage, CameraPtr camera );
	void draw( ) const;
	void reset( );
	Vector getCentralPos( ) const;
private:
	void move( CameraPtr camera );
	void updateState( CameraPtr camera );
	void holdCrystal( StagePtr stage );
	void moveTranslation( const Vector& camera_dir, const Vector& right, const Vector& left );
	void moveScale( Vector scale_left, Vector scale_right );
	void moveRotation( const Vector& camera_dir, Vector right, Vector left );
	void checkLeftRight( CameraPtr camera );
private:
	enum BALL {
		BALL_LEFT,
		BALL_RIGHT
	};
private:
	int _neutral;
	MOVE_STATE _state;
	std::array< BallPtr, 2 > _balls;
	CrystalPtr _crystal;
};

