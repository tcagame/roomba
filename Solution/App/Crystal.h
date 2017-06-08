#pragma once
#include "Drawer.h"
#include "define.h"

PTR( AppStage );
PTR( Viewer );
PTR( Crystal );

class Crystal {
public:
	Crystal( Vector& pos, MDL type );
	virtual ~Crystal( );
public:
	void update( AppStagePtr stage );
	void draw( ViewerPtr viewer ) const;
	bool isHitting( Vector pos0, Vector pos1, Vector vec0, Vector vec1 );
	Vector adjustHitToRoomba( Vector pos, Vector vec, double radius );
	bool isFinished( ) const;
	void setDropDown( bool drop_down );
	Vector getPos( ) const;
	void setVec( Vector& vec );
	bool isDropDown( ) const;
	void shiftPos( Vector& base_pos );
private:
	void toBound( );
private:
	int _effect_count;
	Vector _pos;
	const Vector _start_pos;
	Vector _vec;
	bool _finished;
	bool _drop_down;
	MDL _type;
};

