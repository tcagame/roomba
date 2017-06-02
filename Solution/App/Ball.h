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
	void update( const Vector& vec, StagePtr stage );
	void draw( ) const;
	void reset( const Vector& pos );
	void setPos( Vector pos );
	bool isReflection( ) const;
	Vector getPos( ) const;
	Vector getVec( ) const;
private:
	bool _reflection;
	Vector _pos;
	Vector _vec;
	Vector _before_vec;
};

