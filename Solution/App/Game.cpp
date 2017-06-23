#include "Game.h"
#include "Application.h"
#include "Scene.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneStage.h"
#include "SceneResult.h"
#include "SceneGameOver.h"
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
_next( Scene::NEXT_STAGE ),
_stage_num( 0 ),
_result_time( 0 ) {
}

Game::~Game( ) {
}

void Game::initialize( ) {
	//drawer‰Šú‰»Œã
	changeScene( );
}

void Game::update( ) {
	_next = _scene->update( );
	changeScene( );
}

void Game::changeScene( ) {
	if ( _next == Scene::NEXT_CONTINUE ) {
		return;
	}
	_scene.~shared_ptr( );
	_scene.reset( );

	switch ( _next ) {
	case Scene::NEXT_TITLE:
		_scene = ScenePtr( new SceneTitle );
		break;
	case Scene::NEXT_STAGE_SELECT:
		_scene = ScenePtr( new SceneSelect );
		break;
	case Scene::NEXT_STAGE:
		_scene = ScenePtr( new SceneStage( _stage_num ) );
		break;
	case Scene::NEXT_RESULT:
		_scene = ScenePtr( new SceneResult( _result_time, _col_num ) );
		break;
	case Scene::NEXT_GAMEOVER:
		_scene = ScenePtr( new SceneGameOver( _stage_num ) );
	}
}

void Game::setStage( int stage_num ) {
	_stage_num = stage_num;
	if ( stage_num < 0 ||
		 stage_num > 3 ) {
		_stage_num = 0;
	}
}

void Game::setResultTime( int time ) {
	_result_time = time;
}

void Game::setCollisionNum( int col_num ) {
	_col_num = col_num;
}
