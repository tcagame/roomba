#include "Game.h"
#include "Application.h"
#include "Drawer.h"
#include "Camera.h"
#include "Roomba.h"
#include "Stage.h"
#include "Timer.h"
#include "define.h"
#include "Device.h"

static const double STAGE_MODEL_SIZE = 4;

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
	//drawer��������
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
	drawer->loadGraph( GRAPH_SELECT_MENU, "UI/UI_retry_select.png" );
	
	Matrix size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE / 2 ) ); 
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", size );
	drawer->loadMDLModel( MDL_BG, "Model/Stage/bg.mdl", "Model/Stage/bg01_DM.jpg" );
	drawer->loadMDLModel( MDL_BALL, "Model/Roomba/roomba.mdl", "Model/Roomba/texture.jpg", size );
	
	size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE ) );
	
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", size );

	drawer->loadMDLModel( MDL_WALL_0_0 , "Model/Stage/0_0.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_1 , "Model/Stage/0_1.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_2 , "Model/Stage/0_2.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_3 , "Model/Stage/0_3.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_4 , "Model/Stage/0_4.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_5 , "Model/Stage/0_5.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_6 , "Model/Stage/0_6.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_7 , "Model/Stage/0_7.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_8 , "Model/Stage/0_8.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_9 , "Model/Stage/0_9.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_10, "Model/Stage/0_10.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_11, "Model/Stage/0_11.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_12, "Model/Stage/0_12.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_13, "Model/Stage/0_13.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_14, "Model/Stage/0_14.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_15, "Model/Stage/0_15.mdl", "Model/Stage/wall.jpg", size );

	drawer->loadMDLModel( MDL_WALL_1_0 , "Model/Stage/1_0.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_1 , "Model/Stage/1_1.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_2 , "Model/Stage/1_2.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_3 , "Model/Stage/1_3.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_4 , "Model/Stage/1_4.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_5 , "Model/Stage/1_5.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_6 , "Model/Stage/1_6.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_7 , "Model/Stage/1_7.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_8 , "Model/Stage/1_8.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_9 , "Model/Stage/1_9.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_10, "Model/Stage/1_10.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_11, "Model/Stage/1_11.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_12, "Model/Stage/1_12.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_13, "Model/Stage/1_13.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_14, "Model/Stage/1_14.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_15, "Model/Stage/1_15.mdl", "Model/Stage/wall.jpg", size );


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
		//�I�������烊�Z�b�g
		_roomba->reset( );
		_stage->reset( );
		_camera->reset( );
		_timer->reset( );
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
		drawer->drawString( 700, 400, "���[�ނ��肠" );
	}
	if ( _state == STATE_SELECT_RETRY ) {
		//�I�����
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