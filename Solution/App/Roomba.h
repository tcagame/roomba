#pragma once
#include "Drawer.h"
#include "Stage.h"
#include "Animation.h"
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
		MOVE_STATE_REFLECTION_RESTORE,
		MOVE_STATE_NEUTRAL,
		MOVE_STATE_LIFT_UP,
		MOVE_STATE_LIFT_DOWN,
		MOVE_STATE_WAIT,
		MOVE_STATE_STARTING,
		MAX_STATE,
	};
public:
	Roomba( );
	virtual ~Roomba( );
public:
	void update( StagePtr stage, CameraPtr camera );
	void updateLaser( CameraConstPtr camera );
	void draw( ) const;
	void reset( );
	void setWaitCount( );
	double getRotSpeed( ) const;
	const double getMaxSpeed( ) const;
	Vector getDir( ) const;
	Vector getCentralPos( ) const;
	Vector getBallPos( int ball ) const;
	MOVE_STATE getMoveState( ) const;
	bool isWait( ) const;
	bool isFinished( ) const;
	Vector getStartPos( ) const;
	CrystalPtr getCrystalPtr( ) const;
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
	void updateBalls( StagePtr stage );
	void changeState( StagePtr stage, CameraPtr camera );
	void moveTranslation( );
	void moveRotation( );
	void moveReflection( );
	void moveRestore( );
	void moveLiftUp( );
	void moveLiftDown( );
	void moveBound( );
	void moveStarting( );
	void moveWait( );
	void acceleration( );
	void accelTranslation( );
	void accelRotation( DIR dir );
	void brakeTranslation( );
	void brakeRotation( );
	void holdCrystal( StagePtr stage );
	void checkLeftRight( CameraPtr camera );
	void setVecTrans( Vector& vec );
	void setVecRot( Vector& vec_left, Vector& vec_right );
	void setVecScale( Vector& vec_left, Vector& vec_right );
	void setVecReflection( Vector& vec_left, Vector& vec_right );
	void shiftPos( CameraPtr camera );
	void announceChangeState( MOVE_STATE state );
private:
	int _start_count;
	int _wait_count;
	double _rot_speed;
	bool _link_break;
	bool _finished;
	Vector _trans_speed;
	Vector _move_dir;
	std::array< Vector, 2 > _vec_trans;
	std::array< Vector, 2 > _vec_rot;
	std::array< Vector, 2 > _vec_scale;
	std::array< Vector, 2 > _vec_reflection;
	std::array< double, 2 > _vec_z;
	MOVE_STATE _state;
	std::array< BallPtr, 2 > _balls;
	LaserPtr _laser;
	CrystalPtr _crystal;
	Matrix _stick_rot;
	std::array< AnimationPtr, 2 > _delivery;
};

