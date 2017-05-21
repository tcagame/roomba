#pragma once
#include "Drawer.h"
#include "define.h"

PTR( Viewer );

class Viewer {
public:
	Viewer( );
	virtual ~Viewer( );
public:
	void drawModelMDL( Drawer::ModelMDL mdl ) const;
	void drawModelMDLTransfer( Drawer::ModelMDL mdl ) const;
private:
	char getInViewFlag( Vector pos ) const;
};

