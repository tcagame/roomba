#pragma once
#include "Scene.h"
#include "mathmatics.h"
#include <array>

class SceneResult : public Scene {//ƒŠƒUƒ‹ƒg
public:
	SceneResult( int crystal_carry_num );
	virtual ~SceneResult( );
private:
	NEXT update( );
	void draw( ) const;
	void selectRetry( );
	void drawFrame( ) const;
	void drawCircle( ) const;
	void drawResult( ) const;
	void drawRetry( ) const;
	void drawCrystalNum( ) const;
	void moveChoice( );
private:
	int _count;
	int _choice_count;
	int _move_count;
	int _crystal_carry_num;
	bool _retry;
	std::array < Vector, 2 > _select_pos; // 0:yes 1:no
};

