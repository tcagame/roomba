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
		MOVE_STATE_ROTATION_RIGHT,
		MOVE_STATE_ROTATION_LEFT,
		MOVE_STATE_REFLECTION,
		MOVE_STATE_NEUTRAL,
		MAX_STATE,
	};
public:
	Roomba( );
	virtual ~Roomba( );
public:
	void update( StagePtr stage, CameraPtr camera );
	void draw( ) const;
	void reset( );
	Vector getDir( ) const;
	Vector getCentralPos( ) const;
	Vector getBallPos( int ball ) const;
private:
	enum BALL {
		BALL_LEFT,
		BALL_RIGHT
	};
	enum DIR {
		DIR_RIGHT,
		DIR_LEFT,
	};
private:
	void updateState( );
	void changeState( CameraPtr camera );
	void holdCrystal( StagePtr stage );
	void moveTranslation( );
	void moveRotation( );
	void moveReflection( );
	void acceleration( );
	void accelTranslation( );
	void accelRotation( DIR dir );
	void brakeTranslation( );
	void brakeRotation( );
	void checkLeftRight( CameraPtr camera );
	void setVecTrans( Vector vec );
	void setVecRot( Vector vec_left, Vector vec_right );
	void setVecScale( Vector vec_left, Vector vec_right );
	void setVecReflection( Vector vec_left, Vector vec_right );
	void updateBalls( StagePtr stage );
	void shiftPos( );
	void drawLaser( ) const;
private:
	double _rot_speed;
	Vector _trans_speed;
	Vector _move_dir;
	Vector _vec_trans;
	std::array< Vector, 2 > _vec_rot;
	std::array< Vector, 2 > _vec_scale;
	std::array< Vector, 2 > _vec_reflection;
	MOVE_STATE _state;
	std::array< BallPtr, 2 > _balls;
	CrystalPtr _crystal;
};

