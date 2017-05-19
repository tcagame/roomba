#include "SceneStage.h"
#include "Device.h"
#include "Drawer.h"
#include "AppCamera.h"
#include "Roomba.h"
#include "AppStage.h"
#include "Timer.h"
#include "define.h"
#include "Crystal.h"
#include "Application.h"

static const int UI_PHASE_FOOT_X = 60;
static const int UI_PHASE_Y = 100;
static const int UI_STATION_FOOT_X = 60;
static const int UI_STATION_Y = 200;
static const int UI_NUM_WIDTH = 32;
static const int UI_NUM_HEIGHT = 64;
static const int UI_MAP_SIZE = 6;
static const int UI_MAP_X = 100;
static const int UI_MAP_FOOT_Y = 50;

SceneStage::SceneStage( int stage_num ) :
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
	drawer->loadGraph( GRAPH_MAP, "UI/map.png" );
	Matrix size = Matrix::makeTransformScaling( Vector( WORLD_SCALE, WORLD_SCALE, WORLD_SCALE ) ); 
	drawer->loadMDLModel( MDL_STATION_0, "Model/Station/station.mdl", "Model/Station/purple.jpg", size );
	drawer->loadMDLModel( MDL_STATION_1, "Model/Station/station.mdl", "Model/Station/green.jpg", size );
	drawer->loadMDLModel( MDL_STATION_2, "Model/Station/station.mdl", "Model/Station/blue.jpg", size );
	drawer->loadMDLModel( MDL_STATION_3, "Model/Station/station.mdl", "Model/Station/red.jpg", size );
	drawer->loadMDLModel( MDL_STATION_4, "Model/Station/station.mdl", "Model/Station/yellow.jpg", size );

	Matrix crystal_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE / 2 ) ); 
	drawer->loadMDLModel( MDL_CRYSTAL_0, "Model/Crystal/crystal.mdl", "Model/Crystal/purple.jpg", crystal_size );
	drawer->loadMDLModel( MDL_CRYSTAL_1, "Model/Crystal/crystal.mdl", "Model/Crystal/green.jpg", crystal_size );
	drawer->loadMDLModel( MDL_CRYSTAL_2, "Model/Crystal/crystal.mdl", "Model/Crystal/blue.jpg", crystal_size );
	drawer->loadMDLModel( MDL_CRYSTAL_3, "Model/Crystal/crystal.mdl", "Model/Crystal/red.jpg", crystal_size );
	drawer->loadMDLModel( MDL_CRYSTAL_4, "Model/Crystal/crystal.mdl", "Model/Crystal/yellow.jpg", crystal_size );

	drawer->loadMDLModel( MDL_BG, "Model/Stage/bg.mdl", "Model/Stage/bg01_DM.jpg" );
	
	Matrix roomba_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE ) );
	drawer->loadMDLModel( MDL_BALL, "Model/Roomba/roomba.mdl", "Model/Roomba/texture.jpg", roomba_size );
	Matrix stage_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE ) );
	
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", stage_size );

	drawer->loadMDLModel( MDL_WALL_0_0 , "Model/Stage/0_0.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_1 , "Model/Stage/0_1.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_2 , "Model/Stage/0_2.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_3 , "Model/Stage/0_3.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_4 , "Model/Stage/0_4.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_5 , "Model/Stage/0_5.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_6 , "Model/Stage/0_6.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_7 , "Model/Stage/0_7.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_8 , "Model/Stage/0_8.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_9 , "Model/Stage/0_9.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_10, "Model/Stage/0_10.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_11, "Model/Stage/0_11.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_12, "Model/Stage/0_12.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_13, "Model/Stage/0_13.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_14, "Model/Stage/0_14.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_15, "Model/Stage/0_15.mdl", "Model/Stage/wall.jpg", stage_size );

	drawer->loadMDLModel( MDL_WALL_1_0 , "Model/Stage/1_0.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_1 , "Model/Stage/1_1.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_2 , "Model/Stage/1_2.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_3 , "Model/Stage/1_3.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_4 , "Model/Stage/1_4.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_5 , "Model/Stage/1_5.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_6 , "Model/Stage/1_6.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_7 , "Model/Stage/1_7.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_8 , "Model/Stage/1_8.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_9 , "Model/Stage/1_9.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_10, "Model/Stage/1_10.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_11, "Model/Stage/1_11.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_12, "Model/Stage/1_12.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_13, "Model/Stage/1_13.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_14, "Model/Stage/1_14.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_15, "Model/Stage/1_15.mdl", "Model/Stage/wall.jpg", stage_size );
}


SceneStage::~SceneStage( ) {
}

Scene::NEXT SceneStage::update( ) {
	if ( _state == STATE_NORMAL ) {
		_roomba->update( _stage, _camera );
		_stage->update( );
		_camera->update( );
		_timer->update( );
	
		if ( _timer->isTimeOver( ) ) {
			//_state = STATE_SELECT_RETRY;
			//return NEXT_RETRY;
		}
		if ( _stage->isFinished( ) ) {
			_state = STATE_GAME_CLEAR;
			return NEXT_RESULT;
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
	ApplicationPtr app = Application::getInstance( );
	int scr_width = app->getWindowWidth( );
	int x = scr_width - UI_PHASE_FOOT_X;
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
	x = scr_width - UI_STATION_FOOT_X;
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
	drawMap( );
}

void SceneStage::drawMap( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	int scr_height = app->getWindowHeight( );
	int map_width = UI_MAP_SIZE * STAGE_WIDTH_NUM;
	int map_height = UI_MAP_SIZE * STAGE_HEIGHT_NUM;
	int map_sx = UI_MAP_X;
	int map_sy = scr_height - UI_MAP_FOOT_Y;
	//背景(地面)
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( map_sx, map_sy, 0, 0, 32, 32, map_sx + map_width, map_sy - map_height ), GRAPH_MAP ) );
	Stage::DATA data = _stage->getData( );
	int phase = _stage->getPhase( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		int x = UI_MAP_X + ( ( STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM - i - 1 ) % STAGE_WIDTH_NUM ) * UI_MAP_SIZE;
		int y = map_sy - ( i / STAGE_WIDTH_NUM ) * UI_MAP_SIZE - UI_MAP_SIZE;
		//壁表示
		if ( data.wall[ i ] == 1 ) {
			drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, 32, 0, 32, 32, x + UI_MAP_SIZE, y + UI_MAP_SIZE ), GRAPH_MAP ) );
		}
		//ステーション表示
		if ( data.station[ phase ][ i ] != 0 ) {
			int tx = ( data.station[ phase ][ i ] - 1 ) * 32;
			int ty = 32;
			drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, tx, ty, 32, 32, x + UI_MAP_SIZE, y - UI_MAP_SIZE ), GRAPH_MAP ) );
		}
	}
	//ルンバ表示
	for ( int i = 0; i < 2; i++ ) {
		Vector roomba_pos = _roomba->getBallPos( i );
		int roomba_x = UI_MAP_X + (int)( ( STAGE_WIDTH_NUM - roomba_pos.x / WORLD_SCALE - 0.5 ) * UI_MAP_SIZE );
		int roomba_y = scr_height - UI_MAP_FOOT_Y - (int)( ( roomba_pos.y / WORLD_SCALE + 0.5 ) * UI_MAP_SIZE ) - UI_MAP_SIZE;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( roomba_x, roomba_y, 0, 16 * 5, 16, 16, roomba_x + UI_MAP_SIZE, roomba_y + UI_MAP_SIZE ), GRAPH_MAP ) );
	}
	//クリスタル表示
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	std::list< CrystalPtr > crystals = app_stage->getCrystalList( );
	std::list< CrystalPtr >::const_iterator crystal_ite = crystals.begin( );
	while ( crystal_ite != crystals.end( ) ) {
		CrystalPtr crystal = *crystal_ite;
		if ( !crystal ) {
			crystal_ite++;
			continue;
		}
		Vector pos = crystal->getPos( );
		int type = (int)( crystal->getType( ) - MDL_CRYSTAL_0 );
		int x = UI_MAP_X + (int)( ( STAGE_WIDTH_NUM - pos.x / WORLD_SCALE - 0.5 ) * UI_MAP_SIZE );
		int y = scr_height - UI_MAP_FOOT_Y - (int)( ( pos.y / WORLD_SCALE + 0.5 ) * UI_MAP_SIZE ) - UI_MAP_SIZE;
		int tx = type * 16;
		int ty = 64;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, tx, ty, 16, 16, x + UI_MAP_SIZE, y + UI_MAP_SIZE ), GRAPH_MAP ) );
		crystal_ite++;
	}
}