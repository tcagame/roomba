#pragma once
#include "Scene.h"
#include "mathmatics.h"
#include <array>

class SceneResult : public Scene {//���U���g
public:
	SceneResult( int time, int col_num, bool clear, int crystal_carry_num );
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
	void drawCrystalNum( ) const;
	void moveChoice( );
private:
	int _count;
	int _choice_count;
	int _move_count;
	int _select;
	int _this_time;
	int _best_time;
	int _crystal_carry_num;
	bool _retry;
	bool _moveing;
	bool _stage_clear;
	std::array < Vector, 2 > _select_pos; // 0:yes 1:no
};

