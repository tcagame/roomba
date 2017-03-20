#include "Game.h"
#include "Application.h"
#include "Drawer.h"
#include "Camera.h"
#include "Player.h"
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
	_player = PlayerPtr( new Player );
}

void Game::update( ) {
	_camera->update( );
	_player->update( _stage );
	_stage->draw( );
	_player->draw( );
}