#pragma once
#include "Scene.h"

class SceneGameOver : public Scene {
public:
	SceneGameOver( int stage_num );
	virtual ~SceneGameOver( );
private:
	NEXT update( );
	void draw( );
private:
	int _stage;
	bool _select;
	bool _ispush;
};

