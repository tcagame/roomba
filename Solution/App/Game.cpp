#include "Game.h"
#include "Application.h"
#include "Drawer.h"
#include "Camera.h"
#include "Roomba.h"
#include "Stage.h"
#include "Timer.h"
#include "define.h"
#include "Device.h"

GamePtr Game::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Game >( app->getTask( getTag( ) ) );
}

Game::Game( ) :
	_state( STATE_NORMAL ),
	_select( 0 ) {
	
}


Game::~Game( ) {
}

void Game::initialize( ) {
	//drawer初期化後
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
	drawer->loadGraph( GRAPH_SELECT_MENU, "UI/UI_retry_select.png" );

	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", Matrix::makeTransformScaling( CRYSTAL_SCALE ) );
	drawer->loadMDLModel( MDL_BG, "Model/Stage/bg.mdl", "Model/Stage/bg01_DM.jpg" );

	drawer->loadMV1Model( MV1_BALL, "Model/Roomba/roomba.mv1" );
	drawer->loadMV1Model( MV1_BLOCK_1, "Model/Stage/block_1.mv1" );
	drawer->loadMV1Model( MV1_BLOCK_2, "Model/Stage/block_2.mv1" );
	drawer->loadMDLModel( MDL_WALL_0, "Model/Stage/wall_0.mdl", "Model/Stage/texture.jpg", Matrix::makeTransformScaling( WALL_SCALE ) );
	drawer->loadMDLModel( MDL_WALL_1, "Model/Stage/wall_1.mdl", "Model/Stage/texture.jpg", Matrix::makeTransformScaling( WALL_SCALE ) );
	drawer->loadMDLModel( MDL_WALL_2, "Model/Stage/wall_2.mdl", "Model/Stage/texture.jpg", Matrix::makeTransformScaling( WALL_SCALE ) );

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
		//_timer->update( );

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
		//_timer->reset( );
		_state = STATE_NORMAL;
		DevicePtr device = Device::getTask( );
		if ( device->getDirY( ) > 0 ) {
			_select = 1;
		}
		if ( device->getDirY( ) < 0 ) {
			_select = 0;
		}
		if ( device->getButton( ) & BUTTON_A ) {
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
}

void Game::drawResult( ) {
	if ( _state == STATE_GAME_CLEAR ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 700, 400, "げーむくりあ" );
	}
	if ( _state == STATE_SELECT_RETRY ) {
		//選択画面
		DrawerPtr drawer = Drawer::getTask( );
		ApplicationPtr app = Application::getInstance( );
		int width = app->getWindowWidth( );
		int height = app->getWindowHeight( );
		int select_y = height / 2 + 32 + _select * 64;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 128, height / 2 - 128, 0, 0, 256, 256 ), GRAPH_SELECT_MENU ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 64, height / 2, 256, 0, 128, 128 ), GRAPH_SELECT_MENU ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 64, height / 2 + 64, 256, 0, 128, 128 ), GRAPH_SELECT_MENU ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( width / 2 - 128, select_y, 384, 0, 64, 64 ), GRAPH_SELECT_MENU ) );
	}
}