#include "Game.h"
#include "Application.h"
#include "Scene.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneStage.h"
#include "SceneResult.h"
#include "Drawer.h"
#include "AppCamera.h"
#include "Roomba.h"
#include "Stage.h"
#include "Timer.h"
#include "define.h"

PTR( SceneStage );

GamePtr Game::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Game >( app->getTask( getTag( ) ) );
}

Game::Game( ) :
_next( Scene::NEXT_TITLE ),
_stage_num( 0 ),
_result_time( 0 ),
_stage_clear( false ),
_open_stage( false ) {
}

Game::~Game( ) {
}

void Game::initialize( ) {
	//drawer‰Šú‰»Œã
	changeScene( );
}

void Game::update( ) {
	changeScene( );
	_next = _scene->update( );
}

void Game::changeScene( ) {
	if ( _next == Scene::NEXT_CONTINUE ) {
		return;
	}
	_scene.~shared_ptr( );
	_scene.reset( );
	int crystal_carry_num = 0;
	SceneStagePtr stage = std::dynamic_pointer_cast< SceneStage >( _scene );

	switch ( _next ) {
	case Scene::NEXT_TITLE:
		_scene = ScenePtr( new SceneTitle );
		break;
	case Scene::NEXT_STAGE_SELECT:
		_scene = ScenePtr( new SceneSelect( _open_stage ) );
		break;
	case Scene::NEXT_STAGE:
		_scene = ScenePtr( new SceneStage( _stage_num ) );
		break;
	case Scene::NEXT_RESULT:
		if ( stage ) {
			crystal_carry_num = stage->getCrystalCarryNum( );
		}
		_scene = ScenePtr( new SceneResult( _result_time, _col_num, _stage_clear, crystal_carry_num ) );
		break;
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

void Game::setResult( bool clear ) {
	_stage_clear = clear;
}

void Game::setOpenStage( ) {
	_open_stage = true;
}