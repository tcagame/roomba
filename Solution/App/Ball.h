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
	void update( const Vector& vec, StagePtr stage, bool rot );
	void draw( ) const;
	void reset( const Vector& pos );
	void setPos( Vector& pos );
	bool isReflection( ) const;
	void setReflection( bool ref );
	Vector getPos( ) const;
	Vector getVec( ) const;
private:
	Matrix getMat( const Vector& pos ) const;
private:
	bool _reflection;
	Matrix _rot;
	Vector _pos;
	Vector _vec;
};