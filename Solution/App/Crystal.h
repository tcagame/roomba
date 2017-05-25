#pragma once
#include "Drawer.h"
#include "define.h"

PTR( AppStage );
PTR( Viewer );

class Crystal {
public:
	Crystal( Vector pos, MDL type );
	virtual ~Crystal( );
public:
	void update( AppStagePtr stage );
	void draw( ViewerPtr viewer ) const;
	bool isHitting( Vector pos0, Vector pos1 );
	Vector adjustHitToRoomba( Vector pos, Vector vec, double radius );
	bool isFinished( ) const;
	Vector getPos( ) const;
	void setVec( Vector vec );
	bool isDropDown( ) const;
private:
	Vector _pos;
	Vector _vec;
	bool _finished;
	bool _drop_down;
	MDL _type;
};

