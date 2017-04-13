#include "Game.h"
#include "Application.h"
#include "Drawer.h"
#include "Camera.h"
#include "Roomba.h"
#include "Stage.h"
#include "Timer.h"
#include "define.h"

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
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
	drawer->loadGraph( GRAPH_SELECT_COUSER, "UI/timenumber.png" );
	drawer->loadGraph( GRAPH_SELECT_MENU, "UI/timenumber.png" );

	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", Matrix::makeTransformScaling( CRYSTAL_SCALE ) );
	drawer->loadMDLModel( MDL_BG, "Model/Stage/bg.mdl", "Model/Stage/bg01_DM.jpg" );

	drawer->loadMV1Model( MV1_BALL, "Model/Roomba/roomba.mv1" );
	drawer->loadMV1Model( MV1_WALL_1, "Model/Stage/wall_1.mv1" );
	drawer->loadMV1Model( MV1_WALL_2, "Model/Stage/wall_2.mv1" );
	drawer->loadMV1Model( MV1_BLOCK_1, "Model/Stage/block_1.mv1" );
	drawer->loadMV1Model( MV1_BLOCK_2, "Model/Stage/block_2.mv1" );

	_camera = CameraPtr( new Camera );
	_stage = StagePtr( new Stage );
	_roomba = RoombaPtr( new Roomba );
	_timer = TimerPtr( new Timer );
}

void Game::update( ) {
	if ( _state == STATE_NORMAL ) {
		_roomba->update( _stage, _camera, _timer );
		_stage->update( );
		_camera->update( _roomba );
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
		_roomba->reset( );
		_stage->reset( );
		_camera->reset( );
		_timer->reset( );
		_state = STATE_NORMAL;
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
	if ( _state == STATE_SELECT_RETRY ) {
		DrawerPtr drawer = Drawer::getTask( );
		//drawer->drawString( 700, 400, "げ～むお～ば～" );
		//選択画面
	}
}