#pragma once
#include "Scene.h"

class SceneTitle :	public Scene {//�^�C�g��
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
	void drawManual( );
private:
	int _count;
	int _choice_count;
};

