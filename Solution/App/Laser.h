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
	void show( bool showing );
	void update( const Vector& pos, CameraConstPtr camera, const Vector& left, const Vector& right, CrystalConstPtr crystal );
	void draw( ) const;
private:
	void reset( );
private:
	ModelPtr _model_line;
	ModelPtr _model_hold;
	std::vector< Vector > _particle;
	std::vector< Vector > _guide;
	int _index;
	bool _holding;
	double _hold_r;
	bool _showing;
};

