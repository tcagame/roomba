#pragma once
#include "Drawer.h"
#include "define.h"
#include "Stage.h"

PTR( Viewer );

class Viewer {
public:
	Viewer( );
	virtual ~Viewer( );
public:
	void update( Vector roomba_pos );
	void drawModelMDL( Drawer::ModelMDL mdl ) const;
	void drawModelMDLMulti( ModelPtr mdl, GRAPH graph, int idx = 0 ) const;
	void drawModelMV1( Stage::MV1_INFO& mv1, Matrix scale_rot ) const;
private:
	void setViewPos( Vector& pos ) const;
private:
	Vector _base_pos;
};

