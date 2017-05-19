#pragma once
#include "Drawer.h"
#include "define.h"

PTR( AppStage );
PTR( Stage );

class Ball {
public:
	Ball( const Vector& pos );
	virtual ~Ball( );
public:
	void update( StagePtr stage );
	void draw( ) const;
	void reset( const Vector& pos );
	void setForce( const Vector& force );
	void setPos( Vector pos );
	Vector getPos( ) const;
	Vector getVec( ) const;
private:
	Vector _pos;
	Vector _vec;
};

