#pragma once
#include "Drawer.h"
#include <array>

PTR( Stage );
PTR( Camera );
PTR( AppCamera );
PTR( Ball );
PTR( Crystal );
PTR( Laser );

class Roomba {
public:
	enum MOVE_STATE {
		MOVE_STATE_TRANSLATION,
		MOVE_STATE_ROTATION_RIGHT,
		MOVE_STATE_ROTATION_LEFT,
		MOVE_STATE_REFLECTION,
		MOVE_STATE_RESTORE,
		MOVE_STATE_NEUTRAL,
		MOVE_STATE_LIFT_UP,
		MOVE_STATE_LIFT_DOWN,
		MAX_STATE,
	};
public:
	Roomba( );
	virtual ~Roomba( );
public:
	void update( StagePtr stage, CameraPtr camera );
	void draw( ) const;
	void reset( );
	double getRotSpeed( ) const;
	double getLink( ) const;
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
	void updateLaser( CameraConstPtr camera );
	void updateBalls( StagePtr stage );
	void updateLink( );
	void changeState( CameraPtr camera );
	void moveTranslation( );
	void moveRotation( );
	void moveReflection( );
	void moveRestore( );
	void moveLiftUp( );
	void moveLiftDown( );
	void acceleration( );
	void accelTranslation( );
	void accelRotation( DIR dir );
	void brakeTranslation( );
	void brakeRotation( );
	void holdCrystal( StagePtr stage );
	void checkLeftRight( CameraPtr camera );
	void setVecTrans( Vector vec );
	void setVecRot( Vector vec_left, Vector vec_right );
	void setVecScale( Vector vec_left, Vector vec_right );
	void setVecReflection( Vector vec_left, Vector vec_right );
	void shiftPos( CameraPtr camera );
private:
	double _rot_speed;
	double _link_gauge;
	bool _link_break;
	Vector _trans_speed;
	Vector _move_dir;
	Vector _vec_trans;
	std::array< Vector, 2 > _vec_rot;
	std::array< Vector, 2 > _vec_scale;
	std::array< Vector, 2 > _vec_reflection;
	std::array< Vector, 2 > _vec_lift;
	MOVE_STATE _state;
	std::array< BallPtr, 2 > _balls;
	LaserPtr _laser;
	CrystalPtr _crystal;
	Matrix _stick_rot;
	std::array< Drawer::ModelMDL, 2 > _delivery;
};

