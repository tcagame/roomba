#pragma once
#include "Scene.h"

class SceneSelect : public Scene {//ステージ選択
public:
	SceneSelect( );
	virtual ~SceneSelect( );
private:
	NEXT update( );
	void draw( );
	void drawRogo( );
	void drawSelect( );
private:
	int _select;
	bool _ispush;
};

