#include "Game.h"
#include "Application.h"
#include "Scene.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneStage.h"
#include "Drawer.h"
#include "AppCamera.h"
#include "Roomba.h"
#include "Stage.h"
#include "Timer.h"
#include "define.h"

GamePtr Game::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Game >( app->getTask( getTag( ) ) );
}

Game::Game( ) :
_next( Scene::NEXT_STAGE ) {
	
}


Game::~Game( ) {
}

void Game::initialize( ) {
	//drawer初期化後
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_SELECT_MENU, "UI/UI_retry_select.png" );
	drawer->loadGraph( GRAPH_TITLE, "title/roomb_title.png" );
	drawer->loadGraph( STAGE_SELECT, "select/select.png");
	changeScene( );

}

void Game::update( ) {
	_next = _scene->update( );
	changeScene( );
	/*
	switch ( _next ) {
	case Scene::NEXT_TITLE:
		break;
	case Scene::NEXT_STAGE:
		if ( _state == STATE_NORMAL ) {
			_roomba->update( _stage, _camera, _timer );
			_stage->update( );
			_camera->update( );
			_timer->update( );

			if ( _timer->isTimeOver( ) ) {
				_state = STATE_SELECT_RETRY;
			}
			if ( _stage->isFinished( ) ) {
				_state = STATE_GAME_CLEAR;
			}
		}
		if ( _state == STATE_SELECT_RETRY ) {
			//選択したらリセット
			DevicePtr device = Device::getTask( );
			if ( device->getDirY( ) > 0 ) {
				_select = 1;
			}
			if ( device->getDirY( ) < 0 ) {
				_select = 0;
			}
			if ( device->getButton( ) & BUTTON_D ) {
				_roomba->reset( );
				_stage->reset( );
				_camera->reset( );
				_timer->reset( );
				_state = STATE_NORMAL;
			}
		}
		_stage->draw( );
		_roomba->draw( );
		_timer->draw( );
		drawResult( );
		break;
	}
	*/
}



void Game::changeScene( ) {
	if ( _next == Scene::NEXT_CONTINUE ) {
		return;
	}

	_scene.reset( );

	switch ( _next ) {
	case Scene::NEXT_TITLE:
		_scene = ScenePtr( new SceneTitle );
		break;
	case Scene::NEXT_STAGE_SELECT:
		_scene = ScenePtr( new SceneSelect );
		break;
	case Scene::NEXT_STAGE:
		_scene = ScenePtr( new SceneStage );
		break;
	}
}

