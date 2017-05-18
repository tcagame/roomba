#pragma once
#include "Scene.h"

class SceneRetry : public Scene {
public:
	SceneRetry( int stage_num );
	virtual ~SceneRetry( );
private:
	NEXT update( );
	void draw( );
private:
	int _stage;
	bool _select;
	bool _ispush;
};

