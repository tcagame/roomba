#pragma once
#include "Scene.h"

class SceneTitle :	public Scene {//�^�C�g��
public:
	SceneTitle( );
	virtual ~SceneTitle( );
private:
	NEXT update( );
	void draw( );
	void drawCircle( ) const;
private:
	int _count;
	int _choice_count;
};

