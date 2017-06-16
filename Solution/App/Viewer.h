#pragma once
#include "Drawer.h"
#include "define.h"
#include "Stage.h"

PTR( Viewer );

class Viewer {
public:
	
public:
	Viewer( );
	virtual ~Viewer( );
public:
	void update( Vector roomba_pos );
	void drawModelMDL( Drawer::ModelMDL mdl ) const;
	void drawModelMV1( Stage::MV1_INFO mv1, Matrix scale_rot ) const;
	void drawModelMDLTransfer( Drawer::ModelMDL mdl ) const;
private:
	Vector getViewPos( Vector pos ) const;
private:
	Vector _base_pos;
};

