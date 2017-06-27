#pragma once
#include "Scene.h"
#include "Drawer.h"
#include <array>

class SceneSelect : public Scene {//ステージ選択
public:
	SceneSelect( );
	virtual ~SceneSelect( );
private:
	NEXT update( );
	void draw( );
	void drawRogo( );
	void drawSelect( );
	void moveSelect( );
	void freazeSelect( );
	void drawFrame( );
	void drawTriangle( );
	void drawCircle( ) const;
	void drawBg( );
	void drawOk( );
private:
	std::array< Vector, 3 > _pos;
	int _move_count;
	int _count;
	int _choice_count;
	int _select;
	bool _ispush;
	bool _rot_right;
};

