#pragma once
#include "Scene.h"
class SceneResult : public Scene {//���U���g
public:
	SceneResult( );
	virtual ~SceneResult( );
private:
	NEXT update( );
	void draw( );
};

