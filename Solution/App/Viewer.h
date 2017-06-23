#pragma once
#include "Drawer.h"
#include "define.h"
#include "Stage.h"

PTR( Viewer );
PTR( Shadow );

class Viewer {
public:
	Viewer( ShadowPtr shadow );
	virtual ~Viewer( );
public:
	void update( Vector roomba_pos );
	void drawModelMDL( Drawer::ModelMDL mdl ) const;
	void drawModelMDLMulti( ModelPtr mdl, GRAPH graph, int idx = 0 ) const;
	void drawModelMV1( Stage::MV1_INFO mv1, Matrix scale_rot ) const;
	void setShadow( Vector pos, const double scale, bool sift_pos = true );
private:
	void setViewPos( Vector& pos ) const;
private:
	Vector _base_pos;
	ShadowPtr _shadow;
};

