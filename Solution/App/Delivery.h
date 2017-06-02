#pragma once
#include "Drawer.h"
#include "define.h"

PTR( Camera );
PTR( Crystal );
PTR( Viewer );

class Delivery {
public:
	Delivery( Vector pos );
	virtual ~Delivery( );
public:
	void update( CameraPtr camera );
	void draw( ViewerPtr viewer ) const;
	void setCrystal( Vector pos );
	bool isFinished( ) const;
	bool isHaveCrystal( );
	Vector getPos( ) const;
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
private:
	STATE _state;
	Vector _pos;
	Vector _target;
	Vector _vec;
	bool _finished;
	bool _have_crystal;
	Drawer::ModelMDL _crystal;
};
