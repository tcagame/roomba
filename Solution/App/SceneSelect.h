#pragma once
#include "Scene.h"

class SceneSelect : public Scene {//ステージ選択
public:
	SceneSelect( );
	virtual ~SceneSelect( );
private:
	NEXT update( );
};

