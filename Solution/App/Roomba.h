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
	Vector getBallPos( int ball ) const;
private:
	void move( CameraPtr camera );
	void updateState( CameraPtr camera );
	void holdCrystal( StagePtr stage );
	void moveTranslation( const Vector& camera_dir, const Vector& right, const Vector& left );
	void moveRotation( int rotation_dir );
	void acceleration( Vector right_stick, Vector left_stick );
	void checkLeftRight( CameraPtr camera );
	void setVecTrans( Vector vec_left, Vector vec_right );
	void setVecRot( Vector vec_left, Vector vec_right );
private:
	enum BALL {
		BALL_LEFT,
		BALL_RIGHT
	};
private:
	double _trans_speed;
	double _rot_speed;
	int _rot_dir;
	std::array< Vector, 2 > _vec_trans;
	std::array< Vector, 2 > _vec_rot;
	MOVE_STATE _state;
	std::array< BallPtr, 2 > _balls;
	CrystalPtr _crystal;
};

