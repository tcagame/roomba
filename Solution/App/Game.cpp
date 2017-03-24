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

Game::Game( ) {
}


Game::~Game( ) {
}

void Game::initialize( ) {
	//drawer‰Šú‰»Œã
	_camera = CameraPtr( new Camera );
	_stage = StagePtr( new Stage );
	_roomba = RoombaPtr( new Roomba );
	_timer = TimerPtr( new Timer );
}

void Game::update( ) {
	_roomba->update( _stage, _camera );
	_stage->update( );
	_timer->update( );
	_camera->update( _roomba );
	_stage->draw( );
	_roomba->draw( );
	_timer->draw( );
}