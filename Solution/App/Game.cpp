#include "Game.h"
#include "Application.h"
#include "Drawer.h"
#include "Camera.h"
#include "Roomba.h"
#include "Stage.h"
#include "Timer.h"


GamePtr Game::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Game >( app->getTask( getTag( ) ) );
}

Game::Game( ) :
_state( STATE_NORMAL ) {
	
}


Game::~Game( ) {
}

void Game::initialize( ) {
	//drawer初期化後
	_camera = CameraPtr( new Camera );
	_stage = StagePtr( new Stage );
	_roomba = RoombaPtr( new Roomba );
	_timer = TimerPtr( new Timer );
}

void Game::update( ) {
	if ( _state == STATE_NORMAL ) {
		_roomba->update( _stage, _camera );
		_stage->update( );
		_camera->update( _roomba );
		_timer->update( );
		if ( _timer->isTimeOver( ) ) {
			_state = STATE_GAME_OVER;
		}
		if ( _stage->isFinished( ) ) {
			_state = STATE_GAME_CLEAR;
		}
	}
	_stage->draw( );
	_roomba->draw( );
	_timer->draw( );
	drawResult( );
}

void Game::drawResult( ) {
	if ( _state == STATE_GAME_CLEAR ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 700, 400, "げーむくりあ" );
	}
	if ( _state == STATE_GAME_OVER ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 700, 400, "げーむおーばー" );
	}
}