#pragma once
#include "Scene.h"
class SceneResult : public Scene {//ƒŠƒUƒ‹ƒg
public:
	SceneResult( int time );
	virtual ~SceneResult( );
private:
	NEXT update( );
	void draw( ) const;
	int drawTime( int x, int y, int time ) const;
	void drawThisTime( ) const;
	void drawBestTime( ) const;
	void loadBestTime( );
	void saveBestTime( );
	void drawFrame( ) const;
	void drawCircle( ) const;
private:
	int _choice_count;
	int _select;
	int _this_time;
	int _best_time;
};

