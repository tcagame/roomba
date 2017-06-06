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
#include "RoombaDelivery.h"

static const int UI_PHASE_FOOT_X = 30;
static const int UI_PHASE_Y = 100;
static const int UI_STATION_FOOT_X = 60;
static const int UI_STATION_Y = 200;
static const int UI_NUM_WIDTH = 32;
static const int UI_NUM_HEIGHT = 64;
static const int UI_MAP_SIZE = 6;
static const int UI_MAP_X = 30;
static const int UI_MAP_FOOT_Y = 30;
static const int UI_MAP_RANGE = 20;
static const int START_COUNTDOWN_TIME = 3 * 60;
static const int MAX_LINK_TIME = 400;

SceneStage::SceneStage( int stage_num ) :
_countdown( START_COUNTDOWN_TIME ),
_link_time( 300 ),
_link_break( false ) {	
	_viewer = ViewerPtr( new Viewer );
	_stage = StagePtr( new AppStage( stage_num, _viewer ) );//0-2:通常 3:test_stage
	_roomba = RoombaPtr( new Roomba );
	_camera = CameraPtr( new AppCamera( _roomba ) );
	_timer = TimerPtr( new Timer );
	_roomba_delivery = RoombaDeliveryPtr( new RoombaDelivery );

	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_LINK_GAUGE, "UI/link_gauge.png" );
	drawer->loadGraph( GRAPH_NUMBER, "UI/number.png" );
	drawer->loadGraph( GRAPH_DELIVERY, "UI/station.png" );
	drawer->loadGraph( GRAPH_PHASE, "UI/phase.png" );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
	drawer->loadGraph( GRAPH_MAP, "UI/map.png" );
	Matrix delivery_scale = Matrix::makeTransformScaling( delivery_size );
	drawer->loadMDLModel( MDL_DELIVERY, "Model/Delivery/delivery.mdl", "Model/Delivery/blue.jpg", delivery_scale );

	Matrix crystal_scale = Matrix::makeTransformScaling( crystal_size );
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/purple.jpg", crystal_scale );
		
	Matrix roomba_scale = Matrix::makeTransformScaling( roomba_size );
	//drawer->loadMDLModel( MDL_BALL, "Model/Roomba/roomba.mdl", "Model/Roomba/texture.jpg", roomba_scale );
	drawer->loadMV1Model( 0, "Model/Roomba/roomba.mv1" );
	
	Matrix earth_scale = Matrix::makeTransformScaling( earth_size );
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", earth_scale );

	Matrix stage_scale = Matrix::makeTransformScaling( stage_size );
	drawer->loadMDLModel( MDL_WALL_0_0 , "Model/Stage/0_0.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_1 , "Model/Stage/0_1.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_2 , "Model/Stage/0_2.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_3 , "Model/Stage/0_3.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_4 , "Model/Stage/0_4.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_5 , "Model/Stage/0_5.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_6 , "Model/Stage/0_6.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_7 , "Model/Stage/0_7.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_8 , "Model/Stage/0_8.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_9 , "Model/Stage/0_9.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_10, "Model/Stage/0_10.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_11, "Model/Stage/0_11.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_12, "Model/Stage/0_12.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_13, "Model/Stage/0_13.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_14, "Model/Stage/0_14.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_0_15, "Model/Stage/0_15.mdl", "Model/Stage/wall.jpg", stage_scale );

	drawer->loadMDLModel( MDL_WALL_1_0 , "Model/Stage/1_0.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_1 , "Model/Stage/1_1.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_2 , "Model/Stage/1_2.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_3 , "Model/Stage/1_3.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_4 , "Model/Stage/1_4.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_5 , "Model/Stage/1_5.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_6 , "Model/Stage/1_6.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_7 , "Model/Stage/1_7.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_8 , "Model/Stage/1_8.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_9 , "Model/Stage/1_9.mdl" , "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_10, "Model/Stage/1_10.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_11, "Model/Stage/1_11.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_12, "Model/Stage/1_12.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_13, "Model/Stage/1_13.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_14, "Model/Stage/1_14.mdl", "Model/Stage/wall.jpg", stage_scale );
	drawer->loadMDLModel( MDL_WALL_1_15, "Model/Stage/1_15.mdl", "Model/Stage/wall.jpg", stage_scale );

	drawer->loadEffect( EFFECT_LASER, "Effect/laser.efk" );
	drawer->loadEffect( EFFECT_CATCH_CRYSTAL, "Effect/catch_crystal.efk" );
	drawer->loadEffect( EFFECT_COL_ROOMBA, "Effect/collision_roomba.efk" );
	drawer->loadEffect( EFFECT_CRYSTAL_CIRCLE, "Effect/crystal_effect.efk" );
}


SceneStage::~SceneStage( ) {
}

Scene::NEXT SceneStage::update( ) {
	if ( _countdown / 60 < 1 ) {
		updatePlay( );
	} else {
		countdown( );
		drawCountdown( );
	}

	// カメラ&viwer常に更新する
	_camera->update( );
	_viewer->update( _roomba->getCentralPos( ) );

	if ( _timer->isTimeOver( ) ) {
		return NEXT_RETRY;
	}
	if ( _stage->isFinished( ) ) {
		_timer->finalize( );
		return NEXT_RESULT;
	}
	if ( _roomba_delivery->isDrawRoomba( ) ) {
		_roomba->draw( );
	}
	_stage->draw( );
	_timer->draw( );
	drawUI( );
	return Scene::NEXT_CONTINUE;
}

void SceneStage::countdown( ) {
	_countdown--;
}

void SceneStage::updateRestart( ) {
	_roomba_delivery->update( );
	_roomba_delivery->draw( _roomba );

	if ( _roomba_delivery->isRestart( ) ) {
		_roomba->reset( );
		_roomba_delivery->setPos( _roomba->getBallPos( 0 ), _roomba->getBallPos( 1 ) );
	}

	if ( _roomba_delivery->isReady( ) ) {
		_roomba_delivery->setPos( Vector( ), Vector( ) );
		_link_break = false;
		_link_time = 0;
	}
}

void SceneStage::updatePlay( ) {
	if ( _link_break ) {
		updateRestart( );
	} else {
		_roomba->update( _stage, _camera );
	updateLink( );
	}
	_stage->update( _camera );
	_timer->update( );
}

void SceneStage::updateLink( ) {
	if ( _roomba->isScaling( ) ) {
	//if ( 1 ) {	
		_link_time++;
		if ( _link_time > MAX_LINK_TIME ) {
			_link_time = MAX_LINK_TIME;
			_roomba_delivery->setPos( _roomba->getBallPos( 0 ), _roomba->getBallPos( 1 ) );
			_link_break = true;
		}
	} else {
		_link_time--;
		if ( _link_time < 0 ) {
			_link_time = 0;
		}
	}
}

void SceneStage::drawUI( ) {
	drawUIPhase( );
	drawUIDelivery( );
	// linkgauge
	DrawerPtr drawer = Drawer::getTask( );
	{
		const int TW = 400;
		const int TH = 50;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, 10, 0, TH, TW, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, 10, 0, 0, TW - _link_time, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
	}
	
	drawUIMap( );
}

void SceneStage::drawUIPhase( ) {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	int scr_width = app->getWindowWidth( );
	int x = scr_width - UI_PHASE_FOOT_X;
	int y = UI_PHASE_Y;
	
	//max
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

	//now
	int phase = _stage->getPhase( );
	if ( _phase_number[ 0 ].num != phase ) {
		_phase_number[ 1 ] = _phase_number[ 0 ];
		_phase_number[ 1 ].state = NUMBER_STATE_OUT;
		_phase_number[ 1 ].speed_y = -16;
		_phase_number[ 0 ].num = phase;
		_phase_number[ 0 ].state = NUMBER_STATE_IN;
		_phase_number[ 0 ].x = 0;
		_phase_number[ 0 ].y = 0;
		_phase_number[ 0 ].size = 0;
		_phase_number[ 0 ].speed_y = 0;
	}
	for ( int i = 0; i < 2; i++ ) {
		if ( _phase_number[ i ].state == NUMBER_STATE_OUT ) {
			_phase_number[ i ].x += 2;
			_phase_number[ i ].speed_y++;
			_phase_number[ i ].y += _phase_number[ i ].speed_y;
			if ( _phase_number[ i ].y >= 0 ) {
				_phase_number[ i ].speed_y *= -1;
			}
		}
		if ( _phase_number[ i ].state == NUMBER_STATE_IN ) {
			_phase_number[ i ].size += 0.05;
			if ( _phase_number[ i ].size > 1 ) {
				_phase_number[ i ].size = 1;
				_phase_number[ i ].state = NUMBER_STATE_WAIT;
			}
		}
		int number = _phase_number[ i ].num;
		int sx = x + _phase_number[ i ].x;
		int sy = y + _phase_number[ i ].y;
		int sx2 = sx + (int)( UI_NUM_WIDTH * _phase_number[ i ].size );
		int sy2 = sy + (int)( UI_NUM_HEIGHT * _phase_number[ i ].size );
		Drawer::Sprite sprite( Drawer::Transform( sx, sy, number * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT, sx2, sy2 ), GRAPH_NUMBER );
		drawer->setSprite( sprite );
	}
	x -= UI_NUM_WIDTH;
	x -= 130;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y ), GRAPH_PHASE ) );

}

void SceneStage::drawUIDelivery( ) {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	int scr_width = app->getWindowWidth( );

	int delivery_num = app_stage->getDeliveryCount( ) - 1;
	int x = scr_width - UI_STATION_FOOT_X;
	int y = UI_STATION_Y;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, _stage->getMaxDeliveryNum( ) * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT ), GRAPH_NUMBER ) );
	x -= UI_NUM_WIDTH;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, 10 * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT ), GRAPH_NUMBER ) );
	x -= UI_NUM_WIDTH;

	if ( _delivery_number[ 0 ].num != delivery_num ) {
		_delivery_number[ 1 ] = _delivery_number[ 0 ];
		_delivery_number[ 1 ].state = NUMBER_STATE_OUT;
		_delivery_number[ 1 ].speed_y = -16;
		_delivery_number[ 0 ].num = delivery_num;
		_delivery_number[ 0 ].state = NUMBER_STATE_IN;
		_delivery_number[ 0 ].x = 0;
		_delivery_number[ 0 ].y = 0;
		_delivery_number[ 0 ].size = 0;
		_delivery_number[ 0 ].speed_y = 0;
	}
	for ( int i = 0; i < 2; i++ ) {
		if ( _delivery_number[ i ].state == NUMBER_STATE_OUT ) {
			_delivery_number[ i ].x += 2;
			_delivery_number[ i ].speed_y++;
			_delivery_number[ i ].y += _delivery_number[ i ].speed_y;
			if ( _delivery_number[ i ].y >= 0 ) {
				_delivery_number[ i ].speed_y *= -1;
			}
		}
		if ( _delivery_number[ i ].state == NUMBER_STATE_IN ) {
			_delivery_number[ i ].size += 0.05;
			if ( _delivery_number[ i ].size > 1 ) {
				_delivery_number[ i ].size = 1;
				_delivery_number[ i ].state = NUMBER_STATE_WAIT;
			}
		}
		int number = _delivery_number[ i ].num;
		int sx = x + _delivery_number[ i ].x;
		int sy = y + _delivery_number[ i ].y;
		int sx2 = sx + (int)( UI_NUM_WIDTH * _delivery_number[ i ].size );
		int sy2 = sy + (int)( UI_NUM_HEIGHT * _delivery_number[ i ].size );
		Drawer::Sprite sprite( Drawer::Transform( sx, sy, number * UI_NUM_WIDTH, 0, UI_NUM_WIDTH, UI_NUM_HEIGHT, sx2, sy2 ), GRAPH_NUMBER );
		drawer->setSprite( sprite );
	}
	x -= UI_NUM_WIDTH;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x - 32, y ), GRAPH_DELIVERY ) );
}


void SceneStage::drawUIMap( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	Vector base_pos = _roomba->getCentralPos( );
	Vector dir = _camera->getDir( );
	dir.z = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( dir ), Vector( 0, -1 ).angle( dir ) );

	ApplicationPtr app = Application::getInstance( );
	int map_central_sx = UI_MAP_X + UI_MAP_SIZE * UI_MAP_RANGE;
	int map_central_sy = app->getWindowHeight( ) - UI_MAP_FOOT_Y - UI_MAP_SIZE * UI_MAP_RANGE;
	const int RANGE = UI_MAP_RANGE * UI_MAP_SIZE / 4 * 3;
	Stage::DATA data = _stage->getData( );
	int phase = _stage->getPhase( );
	{//背景(地面)
		int sx = map_central_sx - UI_MAP_SIZE * UI_MAP_RANGE;
		int sy = map_central_sy - UI_MAP_SIZE * UI_MAP_RANGE;
		int sx2 = sx + UI_MAP_SIZE * UI_MAP_RANGE * 2 + UI_MAP_SIZE;
		int sy2 = sy + UI_MAP_SIZE * UI_MAP_RANGE * 2 + UI_MAP_SIZE;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 128, 0, 128, 128, sx2, sy2 ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.5 ) );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 128, 128, 128, sx2, sy2 ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.5 ) );
	}
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		//デリバー表示
		if ( data.delivery[ phase ][ i ] == app_stage->getDeliveryCount( ) ) {
			int x = i % STAGE_WIDTH_NUM;
			int y = i / STAGE_WIDTH_NUM;
			Vector station_pos( i % STAGE_WIDTH_NUM * WORLD_SCALE + WORLD_SCALE / 2, i / STAGE_WIDTH_NUM * WORLD_SCALE + WORLD_SCALE / 2 ); 
			Vector distance = ( getAdjustPos( station_pos, base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
			double length = distance.getLength( );
			if ( distance.getLength( ) > RANGE ) {
				distance = distance.normalize( ) * RANGE;

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
		if ( distance.getLength( ) > RANGE ) {
			distance = distance.normalize( ) * RANGE;
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
		if ( distance.getLength( ) > RANGE ) {
			distance = distance.normalize( ) * RANGE;
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