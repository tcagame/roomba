#pragma once
#include "Drawer.h"
#include "smart_ptr.h"
#include <array>

PTR( Ball );
PTR( Stage );
PTR( AppCamera );
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
	void update( StagePtr stage, AppCameraPtr camera );
	void draw( ) const;
	void reset( );
	Vector getCentralPos( ) const;
private:
	void move( AppCameraPtr camera );
	void updateState( AppCameraPtr camera );
	void holdCrystal( StagePtr stage );
	void moveTranslation( const Vector& camera_dir, const Vector& right, const Vector& left );
	void moveScale( Vector scale_left, Vector scale_right );
	void moveRotation( const Vector& camera_dir, Vector right, Vector left );
	void checkLeftRight( AppCameraPtr camera );
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

