#pragma once
#include "Drawer.h"
#include "define.h"

PTR( Viewer );

class Viewer {
public:
	Viewer( );
	virtual ~Viewer( );
public:
	void update( Vector roomba_pos );
	void drawModelMDL( Drawer::ModelMDL mdl ) const;
	void drawModelMDLTransfer( Drawer::ModelMDL mdl ) const;
private:
	Vector getViewPos( Vector pos ) const;
private:
	Vector _base_pos;
};

