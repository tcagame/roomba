#include "SceneStage.h"
#include "Device.h"
#include "Drawer.h"
#include "AppCamera.h"
#include "Roomba.h"
#include "Stage.h"
#include "Timer.h"
#include "define.h"




SceneStage::SceneStage( ) :
_state( STATE_NORMAL ) {	
	_stage = StagePtr( new Stage );
	_roomba = RoombaPtr( new Roomba );
	_camera = AppCameraPtr( new AppCamera( _roomba ) );
	_timer = TimerPtr( new Timer );
}


SceneStage::~SceneStage( ) {
}

Scene::NEXT SceneStage::update( ) {
	if ( _state == STATE_NORMAL ) {
		_roomba->update( _stage, _camera );
		_stage->update( );
		_camera->update( );
		//_timer->update( );
	
		/*if ( _timer->isTimeOver( ) ) {
			_state = STATE_SELECT_RETRY;
		}*/
		if ( _stage->isFinished( ) ) {
			_state = STATE_GAME_CLEAR;
		}
	}
	/*
	if ( _state == STATE_SELECT_RETRY ) {
		//選択したらリセット
		DevicePtr device = Device::getTask( );
		if ( device->getButton( ) & BUTTON_D ) {
			_roomba->reset( );
			_stage->reset( );
			_camera->reset( );
			_timer->reset( );
			_state = STATE_NORMAL;
		}
	}*/
	_stage->draw( );
	_roomba->draw( );
	_timer->draw( );

	return Scene::NEXT_CONTINUE;
}