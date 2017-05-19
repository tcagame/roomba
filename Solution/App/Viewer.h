#pragma once
#include "Drawer.h"
#include "define.h"

class Viewer {
public:
	Viewer( );
	virtual ~Viewer( );
public:
	static void drawModelMDL( Drawer::ModelMDL mdl );
	static bool isInViewRange( Vector pos );
};

