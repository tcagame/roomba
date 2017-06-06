#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <vector>

PTR( Model );

class Laser {
public:
	Laser();
	virtual ~Laser();
public:
	void update( const Vector& pos );
	void draw( ) const;
private:
	ModelPtr _model;
	std::vector< Vector > _particle;
};

