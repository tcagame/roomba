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
	void set( const Vector& pos, const double scale );
private:
	int _num;
	static const int MAX_SHADOW_NUM = 100;
	std::array< Vector, MAX_SHADOW_NUM > _pos;
	std::array< double, MAX_SHADOW_NUM > _scale;
	ModelPtr _model;
};

