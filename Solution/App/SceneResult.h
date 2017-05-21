#pragma once
#include "Scene.h"
class SceneResult : public Scene {//ƒŠƒUƒ‹ƒg
public:
	SceneResult( int time );
	virtual ~SceneResult( );
private:
	NEXT update( );
	void draw( ) const;
	void drawTimer( ) const;
private:
	int _time;
};

