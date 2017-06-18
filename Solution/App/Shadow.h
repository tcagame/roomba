#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( Model );

class Shadow {
public:
	Shadow( );
	virtual ~Shadow( );
public:
	void update( );
	void draw( ) const;
	void set( const Vector& pos );
private:
	int _num;
	static const int MAX_SHADOW_NUM = 100;
	std::array< Vector, MAX_SHADOW_NUM > _pos;
	ModelPtr _model;
};

