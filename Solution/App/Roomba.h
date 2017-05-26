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
	void move( );
	void updateState( CameraPtr camera );
	void holdCrystal( StagePtr stage );
	void moveTranslation( );
	void moveRotation( );
	void acceleration( );
	void checkLeftRight( CameraPtr camera );
	void setVecTrans( Vector vec_left, Vector vec_right );
	void setVecRot( Vector vec_left, Vector vec_right );
private:
	enum BALL {
		BALL_LEFT,
		BALL_RIGHT
	};
	enum SCALE {
		SCALE_NONE,
		SCALE_SMALL,
		SCALE_BIG,
	};
private:
	double _trans_speed;
	double _rot_speed;
	int _rot_dir;
	Vector _move_dir;
	SCALE _scale_dir;
	std::array< Vector, 2 > _vec_trans;
	std::array< Vector, 2 > _vec_rot;
	MOVE_STATE _state;
	std::array< BallPtr, 2 > _balls;
	CrystalPtr _crystal;
};

