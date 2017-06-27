#pragma once
#include "Scene.h"
class SceneResult : public Scene {//ƒŠƒUƒ‹ƒg
public:
	SceneResult( int time, int col_num, bool clear );
	virtual ~SceneResult( );
private:
	NEXT update( );
	void draw( ) const;
	int drawTime( int x, int y, int time ) const;
	void drawThisTime( ) const;
	void drawBestTime( ) const;
	void loadBestTime( );
	void saveBestTime( );
	void selectRetry( );
	void drawFrame( ) const;
	void drawCircle( ) const;
	void drawResult( ) const;
	void drawRetry( ) const;
	void drawGameClear( ) const;
private:
	int _count;
	int _choice_count;
	int _select;
	int _this_time;
	int _best_time;
	bool _retry;
	bool _stage_clear;
};

