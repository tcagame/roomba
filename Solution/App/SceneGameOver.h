#pragma once
#include "Scene.h"

class SceneGameOver : public Scene {
public:
	SceneGameOver( int stage_num );
	virtual ~SceneGameOver( );
private:
	NEXT update( );
	void draw( );
	void drawFrame( );
	void drawResult( );
	void drawCircle( ) const;
	void drawGameOver( );
	void drawController( ) const;

private:
	int _count;
	int _move_count;
	int _choice_count;
	int _stage;
	bool _ispush;
};

