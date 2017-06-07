#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <vector>

PTR( Camera );
PTR( Model );
PTR( Crystal );

class Laser {
public:
	Laser();
	virtual ~Laser();
public:
	void update( const Vector& pos, CameraConstPtr camera, const Vector& left, const Vector& right, CrystalConstPtr crystal );
	void draw( ) const;
private:
	ModelPtr _model;
	std::vector< Vector > _particle;
	std::vector< Vector > _guide;
	int _index;
};

