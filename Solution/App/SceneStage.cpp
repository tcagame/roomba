#include "SceneStage.h"
#include "Device.h"
#include "Drawer.h"
#include "AppCamera.h"
#include "Roomba.h"
#include "AppStage.h"
#include "Timer.h"
#include "define.h"

static const int UI_PHASE_X = 1400;
static const int UI_PHASE_Y = 100;
static const int UI_STATION_X = 1400;
static const int UI_STATION_Y = 200;
static const int UI_NUM_WIDTH = 32;
static const int UI_NUM_HEIGHT = 64;

SceneStage::SceneStage( ) :
_state( STATE_NORMAL ) {	
	_stage = StagePtr( new AppStage( 3 ) );//0-2:通常 3:test_stage
	_roomba = RoombaPtr( new Roomba );
	_camera = CameraPtr( new AppCamera( _roomba ) );
	_timer = TimerPtr( new Timer );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_NUMBER, "UI/number.png" );
	drawer->loadGraph( GRAPH_STATION, "UI/station.png" );
	drawer->loadGraph( GRAPH_PHASE, "UI/phase.png" );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
	Matrix size = Matrix::makeTransformScaling( Vector( WORLD_SCALE, WORLD_SCALE, WORLD_SCALE ) ); 
	drawer->loadMDLModel( MDL_STATION_0, "Model/Station/station.mdl", "Model/Station/purple.jpg", size );
	drawer->loadMDLModel( MDL_STATION_1, "Model/Station/station.mdl", "Model/Station/green.jpg", size );
	drawer->loadMDLModel( MDL_STATION_2, "Model/Station/station.mdl", "Model/Station/blue.jpg", size );
	drawer->loadMDLModel( MDL_STATION_3, "Model/Station/station.mdl", "Model/Station/red.jpg", size );
	drawer->loadMDLModel( MDL_STATION_4, "Model/Station/station.mdl", "Model/Station/yellow.jpg", size );

	size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE / 2 ) ); 
	drawer->loadMDLModel( MDL_CRYSTAL_0, "Model/Crystal/crystal.mdl", "Model/Crystal/purple.jpg", size );
	drawer->loadMDLModel( MDL_CRYSTAL_1, "Model/Crystal/crystal.mdl", "Model/Crystal/green.jpg", size );
	drawer->loadMDLModel( MDL_CRYSTAL_2, "Model/Crystal/crystal.mdl", "Model/Crystal/blue.jpg", size );
	drawer->loadMDLModel( MDL_CRYSTAL_3, "Model/Crystal/crystal.mdl", "Model/Crystal/red.jpg", size );
	drawer->loadMDLModel( MDL_CRYSTAL_4, "Model/Crystal/crystal.mdl", "Model/Crystal/yellow.jpg", size );

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
}


SceneStage::~SceneStage( ) {
}

Scene::NEXT SceneStage::update( ) {
	if ( _state == STATE_NORMAL ) {
		_roomba->update( _stage, _camera );
		_stage->update( );
		_camera->update( );
		//_timer->update( );
	
		/*if ( _timer->isTimeOver( ) ) {
			_state = STATE_SELECT_RETRY;
		}*/
		if ( _stage->isFinished( ) ) {
			_state = STATE_GAME_CLEAR;
		}
	}
	/*
	if ( _state == STATE_SELECT_RETRY ) {
		//選択したらリセット
		DevicePtr device = Device::getTask( );
		if ( device->getButton( ) & BUTTON_D ) {
			_roomba->reset( );
			_stage->reset( );
			_camera->reset( );
			_timer->reset( );
			_state = STATE_NORMAL;
		}
	}*/
	_stage->draw( );
	_roomba->draw( );
	_timer->draw( );
	drawUI( );
	return Scene::NEXT_CONTINUE;
}

void SceneStage::drawUI( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	//phase
	int x = UI_PHASE_X;
	int y = UI_PHASE_Y;
	int max = MAX_PHASE -1;
	while ( max >= 0 ) {
		int num = max % 10;
		Drawer::Sprite sprite( Drawer::Transform( x, y, num * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT ), GRAPH_NUMBER );
		drawer->setSprite( sprite );
		
		x -= UI_NUM_WIDTH;
		max /= 10;
		if ( max == 0 ) {
			break;
		}
	};
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, 10 * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT ), GRAPH_NUMBER ) );
	x -= UI_NUM_WIDTH;
	int phase = _stage->getPhase( );
	while ( phase >= 0 ) {
		int num = phase % 10;
		Drawer::Sprite sprite( Drawer::Transform( x, y, num * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT ), GRAPH_NUMBER );
		drawer->setSprite( sprite );
		
		x -= UI_NUM_WIDTH;
		phase /= 10;
		if ( phase == 0 ) {
			break;
		}
	};
	x -= 130;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y ), GRAPH_PHASE ) );

	//station
	int station_num = app_stage->getStationNum( );
	x = UI_STATION_X;
	y = UI_STATION_Y;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, _stage->getMaxStationNum( ) * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT ), GRAPH_NUMBER ) );
	x -= UI_NUM_WIDTH;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, 10 * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT ), GRAPH_NUMBER ) );
	x -= UI_NUM_WIDTH;
	while ( station_num >= 0 ) {
		int num = station_num % 10;
		Drawer::Sprite sprite( Drawer::Transform( x, y, num * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT ), GRAPH_NUMBER );
		drawer->setSprite( sprite );
		
		x -= UI_NUM_WIDTH;
		station_num /= 10;
		if ( station_num == 0 ) {
			break;
		}
	};
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x - 32, y ), GRAPH_STATION ) );

}