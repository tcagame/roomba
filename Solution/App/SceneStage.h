#pragma once
#include "Scene.h"
#include "smart_ptr.h"
#include "Drawer.h"

PTR( RoombaDelivery );
PTR( Roomba );
PTR( Camera );
PTR( Stage );
PTR( AppStage );
PTR( Timer );
PTR( Viewer );

class SceneStage : public Scene {//ステージ
public:
	SceneStage( int stage_num );
	virtual ~SceneStage( );
	Scene::NEXT update( );
private:
	void countdown( );
	void updateRestart( );
	void updatePlay( );
	void updateLink( );
	void drawCountdown( ) const;
	void drawUI( ) const;
	void drawMap( ) const;
	Vector getAdjustPos( Vector pos, Vector base_pos ) const;
private:
	int _countdown;
	int _link_time;
	bool _link_break;
	CameraPtr _camera;
	StagePtr _stage;
	RoombaPtr _roomba;
	TimerPtr _timer;
	ViewerPtr _viewer;
	RoombaDeliveryPtr _roomba_delivery;
};

