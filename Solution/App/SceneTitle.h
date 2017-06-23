#pragma once
#include "Scene.h"

class SceneTitle :	public Scene {//ƒ^ƒCƒgƒ‹
public:
	SceneTitle( );
	virtual ~SceneTitle( );
private:
	NEXT update( );
	void draw( );
	void drawTitle( );
	void drawPlease( );
	void drawController( );
	void drawCircle( ) const;
	void drawBg( );
private:
	int _count;
	int _choice_count;
};

