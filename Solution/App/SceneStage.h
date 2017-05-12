#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( Roomba );
PTR( Camera );
PTR( Stage );
PTR( Timer );

class SceneStage : public Scene {//ステージ
public:
	SceneStage( );
	virtual ~SceneStage( );
	Scene::NEXT update( );
private:
	enum STATE {
		STATE_NORMAL,
		STATE_GAME_CLEAR,
		STATE_SELECT_RETRY,
		MAX_STATE,
	};
private:
	CameraPtr _camera;
	StagePtr _stage;
	RoombaPtr _roomba;
	TimerPtr _timer;
	STATE _state;
};

