#pragma once
#include "Drawer.h"
#include "define.h"
#include "Stage.h"
#include "Animation.h"

PTR( Camera );
PTR( Crystal );
PTR( Viewer );
PTR( Shadow );

class Delivery {
public:
	Delivery( Vector pos );
	virtual ~Delivery( );
public:
	void update( CameraPtr camera, ShadowPtr shadow );
	void draw( ViewerConstPtr viewer ) const;
	void setCrystal( Vector crystal_pos );
	bool isFinished( ) const;
	bool isHaveCrystal( );
	const Vector& getPos( ) const;
private:
	enum STATE {
		STATE_WAIT,
		STATE_CATCH,
		STATE_LIFT,
		STATE_CARRY,
	};
private:
	void updateWait( );
	void updateCatch( );
	void updateLift( CameraPtr camera );
	void updateCarry( );
	void move( );
private:
	int _effect_count;
	STATE _state;
	Vector _target;
	Vector _vec;
	bool _finished;
	bool _have_crystal;
	Drawer::ModelMDL _crystal;
	AnimationPtr _animation;
};
