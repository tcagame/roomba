#include "SceneStage.h"
#include "Device.h"
#include "AppCamera.h"
#include "Roomba.h"
#include "AppStage.h"
#include "Timer.h"
#include "define.h"
#include "Crystal.h"
#include "Application.h"
#include "Game.h"
#include "Viewer.h"

static const int UI_PHASE_FOOT_X = 60;
static const int UI_PHASE_Y = 100;
static const int UI_STATION_FOOT_X = 60;
static const int UI_STATION_Y = 200;
static const int UI_NUM_WIDTH = 32;
static const int UI_NUM_HEIGHT = 64;
static const int UI_MAP_SIZE = 6;
static const int UI_MAP_X = 100;
static const int UI_MAP_FOOT_Y = 50;
static const int UI_MAP_RANGE = 30;
static const int START_COUNTDOWN_TIME = 3 * 60;

SceneStage::SceneStage( int stage_num ) :
_countdown( START_COUNTDOWN_TIME ),
_link_time( 0 ) {	
	_viewer = ViewerPtr( new Viewer );
	_stage = StagePtr( new AppStage( stage_num, _viewer ) );//0-2:通常 3:test_stage
	_roomba = RoombaPtr( new Roomba );
	_camera = CameraPtr( new AppCamera( _roomba ) );
	_timer = TimerPtr( new Timer );

	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_LINK_GAUGE, "UI/link_gauge.png" );
	drawer->loadGraph( GRAPH_NUMBER, "UI/number.png" );
	drawer->loadGraph( GRAPH_STATION, "UI/station.png" );
	drawer->loadGraph( GRAPH_PHASE, "UI/phase.png" );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
	drawer->loadGraph( GRAPH_MAP, "UI/map.png" );
	Matrix size = Matrix::makeTransformScaling( Vector( WORLD_SCALE, WORLD_SCALE, WORLD_SCALE ) ); 
	drawer->loadMDLModel( MDL_DELIVERY, "Model/Delivery/delivery.mdl", "Model/Delivery/blue.jpg", size );

	Matrix crystal_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE / 2 ) ); 
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/purple.jpg", crystal_size );

	
	Matrix roomba_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE ) );
	drawer->loadMDLModel( MDL_BALL, "Model/Roomba/roomba.mdl", "Model/Roomba/texture.jpg", roomba_size );
	
	Matrix earth_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / STAGE_MODEL_SIZE * STAGE_WIDTH_NUM, WORLD_SCALE / STAGE_MODEL_SIZE * STAGE_HEIGHT_NUM, WORLD_SCALE / STAGE_MODEL_SIZE ) );
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", earth_size );
	Matrix stage_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE ) );

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

	drawer->loadEffect( EFFECT_LASER, "Effect/laser.efk" );
}


SceneStage::~SceneStage( ) {
}

Scene::NEXT SceneStage::update( ) {
	if ( _countdown / 60 < 1 ) {
		updateGame( );
	} else {
		countdown( );
		drawCountdown( );
	}

	// カメラだけは常に更新する
	_camera->update( );

	if ( _timer->isTimeOver( ) ) {
		return NEXT_RETRY;
	}
	if ( _stage->isFinished( ) ) {
		_timer->finalize( );
		return NEXT_RESULT;
	}
	_stage->draw( );
	_roomba->draw( );
	_timer->draw( );
	drawUI( );
	return Scene::NEXT_CONTINUE;
}

void SceneStage::countdown( ) {
	_countdown--;
}

void SceneStage::updateGame( ) {
	_roomba->update( _stage, _camera );
	_stage->update( );
	_viewer->update( _roomba->getCentralPos( ) );
	_timer->update( );
	updateTime( );
}

void SceneStage::updateTime( ) {
	if ( _roomba->isScaling( ) ) {		
		_link_time++;
	} else {
		_link_time--;
		if ( _link_time < 0 ) {
			_link_time = 0;
		}
	}
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
	int station_num = app_stage->getDeliveryNum( );
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

	// linkgauge
	{
		const int TW = 400;
		const int TH = 50;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, 10, 0, TH, TW, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, 10, 0, 0, TW - _link_time, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
	}
	
	drawMap( );
}

void SceneStage::drawMap( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	Vector base_pos = _roomba->getCentralPos( );
	Vector dir = _roomba->getDir( );
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( dir ), Vector( 0, -1 ).angle( dir ) );

	ApplicationPtr app = Application::getInstance( );
	int scr_height = app->getWindowHeight( );
	int map_central_sx = UI_MAP_X + UI_MAP_SIZE * UI_MAP_RANGE;
	int map_central_sy = scr_height - UI_MAP_FOOT_Y - UI_MAP_SIZE * UI_MAP_RANGE;
	Stage::DATA data = _stage->getData( );
	int phase = _stage->getPhase( );
	{//背景(地面)
		int sx = map_central_sx - (int)( UI_MAP_SIZE * UI_MAP_RANGE / 1.5 );
		int sy = map_central_sy - (int)( UI_MAP_SIZE * UI_MAP_RANGE / 1.5 );
		int sx2 = sx + UI_MAP_SIZE * (int)( UI_MAP_RANGE + UI_MAP_SIZE );
		int sy2 = sy + UI_MAP_SIZE * (int)( UI_MAP_RANGE + UI_MAP_SIZE );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 128, 0, 128, 128, sx2, sy2 ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.5 ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 128, 128, 128, sx2, sy2 ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.5 ) );
	}
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		//ステーション表示
		if ( data.delivery[ phase ][ i ] == app_stage->getDeliveryCount( ) ) {
			int x = i % STAGE_WIDTH_NUM;
			int y = i / STAGE_WIDTH_NUM;
			Vector station_pos( i % STAGE_WIDTH_NUM * WORLD_SCALE + WORLD_SCALE / 2, i / STAGE_WIDTH_NUM * WORLD_SCALE + WORLD_SCALE / 2 ); 
			Vector distance = ( getAdjustPos( station_pos, base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
			if ( distance.getLength( ) > UI_MAP_RANGE * UI_MAP_SIZE ) {
				break;
			}
			distance = mat.multiply( distance );
			int sx = (int)( map_central_sx + distance.x - WORLD_SCALE / 4 );
			int sy = (int)( map_central_sy + distance.y - WORLD_SCALE / 4 );
			drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 0, 32, 32, sx + UI_MAP_SIZE, sy + UI_MAP_SIZE ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.8 ) );
			break;
		}
	}
	//ルンバ表示
	for ( int i = 0; i < 2; i++ ) {
		Vector distance = ( getAdjustPos( _roomba->getBallPos( i ), base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
		distance = mat.multiply( distance );
		if ( distance.getLength( ) > UI_MAP_RANGE * UI_MAP_SIZE ) {
			break;
		}
		int sx = (int)( map_central_sx + distance.x );
		int sy = (int)( map_central_sy + distance.y );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 48, 16, 16, sx + UI_MAP_SIZE, sy + UI_MAP_SIZE ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.8 ) );
	}
	//クリスタル表示
	std::list< CrystalPtr > crystals = app_stage->getCrystalList( );
	std::list< CrystalPtr >::const_iterator crystal_ite = crystals.begin( );
	while ( crystal_ite != crystals.end( ) ) {
		CrystalPtr crystal = *crystal_ite;
		if ( !crystal ) {
			crystal_ite++;
			continue;
		}
		Vector distance = ( getAdjustPos( crystal->getPos( ), base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
		distance = mat.multiply( distance );
		if ( distance.getLength( ) > UI_MAP_RANGE * UI_MAP_SIZE ) {
			continue;
		}
		int sx = (int)( map_central_sx + distance.x );
		int sy = (int)( map_central_sy + distance.y );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 32, 16, 16, sx + UI_MAP_SIZE, sy + UI_MAP_SIZE ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.8 ) );
		crystal_ite++;
	}
}

void SceneStage::drawCountdown( ) const {
	if ( _countdown / 60 < 1 ) {
		return;
	}
	
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	const int FPS = 60;
	const int sx = WIDTH / 2 - 100;
	const int sy = HEIGHT / 2 - 200;
	const int sx2 = sx + 200;
	const int sy2 = sy + 400;
	const int TW = 32;
	const int TH = 64;
	const int u[ ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	Drawer::Sprite sprite( Drawer::Transform( sx, sy, u[ _countdown / FPS ] * TW, 0, TW, TH, sx2, sy2 ), GRAPH_TIMER_NUM );
	drawer->setSprite( sprite );
}

Vector SceneStage::getAdjustPos( Vector pos, Vector base_pos ) const {
	while ( pos.x - base_pos.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.x - base_pos.x < -STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.y - base_pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	while ( pos.y - base_pos.y < -STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	return pos;
}