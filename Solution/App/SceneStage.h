#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( Roomba );
PTR( Camera );
PTR( Stage );
PTR( AppStage );
PTR( Timer );

class SceneStage : public Scene {//ステージ
public:
	SceneStage( int stage_num );
	virtual ~SceneStage( );
	Scene::NEXT update( );
private:
	void countdown( );
	void updateGame( );
	void drawCountdown( ) const;
	void drawUI( ) const;
	void drawMap( ) const;
private:
	int _countdown;
	CameraPtr _camera;
	StagePtr _stage;
	RoombaPtr _roomba;
	TimerPtr _timer;
};

