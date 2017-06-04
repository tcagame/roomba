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
	void drawUI( );
	void drawUIPhase( );
	void drawUIDelivery( );
	void drawUIMap( ) const;
	Vector getAdjustPos( Vector pos, Vector base_pos ) const;
private:
	enum NUMBER_STATE {
		NUMBER_STATE_IN,
		NUMBER_STATE_WAIT,
		NUMBER_STATE_OUT,
	};
	struct NUMBER {
		int x;
		int y;
		int speed_y;
		int num;
		double size;
		NUMBER_STATE state;
	};
private:
	int _countdown;
	int _link_time;
	bool _link_break;
	NUMBER _phase_number[ 2 ];
	NUMBER _delivery_number[ 2 ];
	CameraPtr _camera;
	StagePtr _stage;
	RoombaPtr _roomba;
	TimerPtr _timer;
	ViewerPtr _viewer;
	RoombaDeliveryPtr _roomba_delivery;
};

