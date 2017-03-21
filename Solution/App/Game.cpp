#include "Game.h"
#include "Application.h"
#include "Drawer.h"
#include "Camera.h"
#include "Roomba.h"
#include "Stage.h"

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
	_Roomba = RoombaPtr( new Roomba );
}

void Game::update( ) {
	_Roomba->update( _stage );
	_camera->update( _Roomba );
	_stage->draw( );
	_Roomba->draw( );
}