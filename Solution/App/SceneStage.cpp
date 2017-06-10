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
#include "Sound.h"

static const int UI_DELIVERY_FOOT_X = 80;
static const int UI_DELIVERY_Y = 20;
static const int UI_NUM_WIDTH = 32;
static const int UI_NUM_HEIGHT = 64;
static const int UI_MAP_SIZE = 6;
static const int UI_MAP_X = 30;
static const int UI_MAP_FOOT_Y = 30;
static const int UI_MAP_RANGE = 20;
static const int START_COUNTDOWN_TIME = 120;
static const int ERASE_COUNTDOWN_TIME = 15;

SceneStage::SceneStage( int stage_num ) :
_countdown( START_COUNTDOWN_TIME ) {	

	_viewer = ViewerPtr( new Viewer );
	_timer = TimerPtr( new Timer );
	_stage = StagePtr( new AppStage( stage_num, _viewer, _timer ) );//0-2:通常 3:test_stage
	_roomba = RoombaPtr( new Roomba );
	_camera = CameraPtr( new AppCamera( _roomba ) );
	

	_delivery_number[ 0 ].state = NUMBER_STATE_IN;
	_delivery_number[ 1 ].state = NUMBER_STATE_NONE;
	_delivery_number[ 0 ].num = _stage->getMaxDeliveryNum( ) - std::dynamic_pointer_cast<AppStage>( _stage )->getDeliveryCount( ) + 1;
	_phase_number[ 0 ].state = NUMBER_STATE_IN;
	_phase_number[ 1 ].state = NUMBER_STATE_NONE;

	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_LINK_GAUGE, "UI/link_gauge.png" );
	drawer->loadGraph( GRAPH_NUMBER, "UI/number.png" );
	drawer->loadGraph( GRAPH_DELIVERY, "UI/station.png" );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
	drawer->loadGraph( GRAPH_MAP, "UI/map.png" );
	drawer->loadGraph( GRAPH_MATRIX, "UI/matrix.png" );
	drawer->loadGraph( GRAPH_MATRIX_ERASE, "UI/matrix_erase.png" );
	Matrix delivery_scale = Matrix::makeTransformScaling( DELIVERY_SIZE );
	drawer->loadMDLModel( MDL_DELIVERY, "Model/Delivery/delivery.mdl", "Model/Delivery/blue.jpg", delivery_scale );

	Matrix crystal_scale = Matrix::makeTransformScaling( CRYSTAL_SIZE );
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/purple.jpg", crystal_scale );
		
	Matrix roomba_scale = Matrix::makeTransformScaling( ROOMBA_SIZE );
	//drawer->loadMDLModel( MDL_BALL, "Model/Roomba/roomba.mdl", "Model/Roomba/texture.jpg", roomba_scale );
	drawer->loadMV1Model( 0, "Model/Roomba/roomba.mv1" );
	
	Matrix earth_scale = Matrix::makeTransformScaling( EARTH_SIZE );
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", earth_scale );

	Matrix wall_scale = Matrix::makeTransformScaling( WALL_SIZE );
	drawer->loadMDLModel( MDL_WALL_0_0 , "Model/Stage/0_0.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_1 , "Model/Stage/0_1.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_2 , "Model/Stage/0_2.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_3 , "Model/Stage/0_3.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_4 , "Model/Stage/0_4.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_5 , "Model/Stage/0_5.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_6 , "Model/Stage/0_6.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_7 , "Model/Stage/0_7.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_8 , "Model/Stage/0_8.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_9 , "Model/Stage/0_9.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_10, "Model/Stage/0_10.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_11, "Model/Stage/0_11.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_12, "Model/Stage/0_12.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_13, "Model/Stage/0_13.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_14, "Model/Stage/0_14.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_0_15, "Model/Stage/0_15.mdl", "Model/Stage/wall.jpg", wall_scale );

	drawer->loadMDLModel( MDL_WALL_1_0 , "Model/Stage/1_0.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_1 , "Model/Stage/1_1.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_2 , "Model/Stage/1_2.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_3 , "Model/Stage/1_3.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_4 , "Model/Stage/1_4.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_5 , "Model/Stage/1_5.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_6 , "Model/Stage/1_6.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_7 , "Model/Stage/1_7.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_8 , "Model/Stage/1_8.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_9 , "Model/Stage/1_9.mdl" , "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_10, "Model/Stage/1_10.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_11, "Model/Stage/1_11.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_12, "Model/Stage/1_12.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_13, "Model/Stage/1_13.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_14, "Model/Stage/1_14.mdl", "Model/Stage/wall.jpg", wall_scale );
	drawer->loadMDLModel( MDL_WALL_1_15, "Model/Stage/1_15.mdl", "Model/Stage/wall.jpg", wall_scale );

	drawer->loadEffect( EFFECT_LASER, "Effect/laser.efk" );
	drawer->loadEffect( EFFECT_CATCH_CRYSTAL, "Effect/catch_crystal.efk" );
	drawer->loadEffect( EFFECT_COL_ROOMBA, "Effect/collision_roomba.efk" );
	drawer->loadEffect( EFFECT_COLLISION_TO_CRYSTAL, "Effect/collision_crystal.efk" );
	drawer->loadEffect( EFFECT_CRYSTAL_CIRCLE, "Effect/crystal_effect.efk" );
	drawer->loadEffect( EFFECT_CHANGE_ROOMBA_STATE, "Effect/move_roomba.efk" );
	drawer->loadEffect( EFFECT_DELIVERY_POINT, "Effect/point.efk" );
	drawer->loadEffect( EFFECT_REPLAY, "Effect/replay.efk" );

	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "bgm_maoudamashii_cyber06.wav" );
}


SceneStage::~SceneStage( ) {
}

Scene::NEXT SceneStage::update( ) {
	// カメラ&viwer常に更新する
	_camera->update( );
	_viewer->update( _roomba->getCentralPos( ) );

	if ( _timer->isTimeOver( ) ) {
		return NEXT_RETRY;
	}
	if ( std::dynamic_pointer_cast<AppStage>(_stage)->isFinished( ) ) {
		_timer->finalize( );
		return NEXT_RESULT;
	}
	if ( _countdown < 0 ) {
		updatePlay( );
		_roomba->draw( );
		_stage->draw( );
		_timer->draw( );
		drawUI( );
	} else {
		//countdown( );
		_countdown--;
		drawCountdown( );
	}

	return Scene::NEXT_CONTINUE;
}

void SceneStage::countdown( ) {
	_countdown--;
}

void SceneStage::updateRestart( ) {
}

void SceneStage::updatePlay( ) {
	_roomba->update( _stage, _camera );
	_stage->update( _camera );
	_roomba->updateLaser( _camera );
	_timer->update( );
}

void SceneStage::drawUI( ) {
	drawUIDelivery( );
	drawUILinKGauge( );
	drawUIMap( );
}

void SceneStage::drawUILinKGauge( ) {
	DrawerPtr drawer = Drawer::getTask( );
	const int TW = 400;
	const int TH = 50;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, 10, 0, TH, TW, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, 10, 0, 0, (int)_roomba->getLink( ), TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
}

void SceneStage::drawUIDelivery( ) {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	int scr_width = app->getWindowWidth( );

	int delivery_num = _stage->getMaxDeliveryNum( ) - app_stage->getDeliveryCount( ) + 1;
	
	int x = scr_width - UI_DELIVERY_FOOT_X;
	int y = UI_DELIVERY_Y;
	
	if ( _delivery_number[ 0 ].num != delivery_num ) {
		_delivery_number[ 1 ] = _delivery_number[ 0 ];
		_delivery_number[ 1 ].state = NUMBER_STATE_OUT;
		_delivery_number[ 1 ].speed_y = -10;
		_delivery_number[ 0 ].num = delivery_num;
		_delivery_number[ 0 ].state = NUMBER_STATE_IN;
		_delivery_number[ 0 ].x = 0;
		_delivery_number[ 0 ].y = 0;
		_delivery_number[ 0 ].size = 0;
		_delivery_number[ 0 ].speed_y = 0;
	}
	for ( int i = 0; i < 2; i++ ) {
		if ( _delivery_number[ i ].state == NUMBER_STATE_NONE ) {
			continue;
		}
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
	x -= 64;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y ), GRAPH_DELIVERY ) );
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
		if ( _stage->getData( i ).delivery == app_stage->getDeliveryCount( ) ) {
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
	if ( _countdown < 0 ) {
		return;
	}
	/*
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
	*/
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	const int IMG_HEIGHT = 992;
	int width = WIDTH / 2;
	int height = HEIGHT / 2;
	int line = 0;
	if ( _countdown < ERASE_COUNTDOWN_TIME ) {
		line = ( START_COUNTDOWN_TIME - ERASE_COUNTDOWN_TIME ) % ( ( IMG_HEIGHT - 540 ) / 32 );
		width = ( ERASE_COUNTDOWN_TIME - _countdown % ERASE_COUNTDOWN_TIME ) * ( WIDTH / ERASE_COUNTDOWN_TIME ) * 5;
		height = ( ERASE_COUNTDOWN_TIME - _countdown % ERASE_COUNTDOWN_TIME ) * ( HEIGHT / ERASE_COUNTDOWN_TIME ) * 5;
		int sx1 = WIDTH / 2 - width;
		int sx2 = WIDTH / 2 + width;
		int sy1 = HEIGHT / 2 - height;
		int sy2 = HEIGHT / 2 + height;
		int random_x = rand( ) % 300 - 150;
		int random_y = rand( ) % 300 - 150;
		Drawer::Transform trans( 0, 0, 0, ( line * 32 ), 960, 540, WIDTH, HEIGHT );
		drawer->setSprite( Drawer::Sprite( trans, GRAPH_MATRIX ) );
		trans = Drawer::Transform( 0, 0, 0, 976, 960, 16, WIDTH, 16 * WIDTH / 960 );
		drawer->setSprite( Drawer::Sprite( trans, GRAPH_MATRIX ) );
		Drawer::Transform cover( -width / 2 - random_x, -height / 2 - random_y, 0, 0, 1280, 720, width / 2 - random_x, height / 2 - random_y );
		drawer->setSprite( Drawer::Sprite( cover, GRAPH_MATRIX_ERASE ) );
		cover = Drawer::Transform( WIDTH - width / 2 + random_x, -height / 2 - random_y, 0, 0, 1280, 720, WIDTH + width / 2 + random_x, height / 2 - random_y );
		drawer->setSprite( Drawer::Sprite( cover, GRAPH_MATRIX_ERASE ) );
		cover = Drawer::Transform( -width / 2 - random_x, HEIGHT - height / 2 + random_y, 0, 0, 1280, 720, width / 2 - random_x, HEIGHT + height / 2 + random_y );
		drawer->setSprite( Drawer::Sprite( cover, GRAPH_MATRIX_ERASE ) );
		cover = Drawer::Transform( WIDTH -width / 2 + random_x, HEIGHT - height / 2 + random_y, 0, 0, 1280, 720, WIDTH + width / 2 + random_x, HEIGHT + height / 2 + random_y );
		drawer->setSprite( Drawer::Sprite( cover, GRAPH_MATRIX_ERASE ) );
	} else {
		int th = ( START_COUNTDOWN_TIME - _countdown ) * 16;
		if ( th > 540 ) {
			th = 540;
		}
		line = ( START_COUNTDOWN_TIME - _countdown ) % ( ( IMG_HEIGHT - 540 ) / 32 );
		Drawer::Transform trans( 0, 0, 0, ( line * 32 ), 960, th, WIDTH, th * HEIGHT / 540 );
		drawer->setSprite( Drawer::Sprite( trans, GRAPH_MATRIX ) );
		trans = Drawer::Transform( 0, 0, 0, 976, 960, 16, WIDTH, 16 * WIDTH / 960 );
		drawer->setSprite( Drawer::Sprite( trans, GRAPH_MATRIX ) );
	}
	//Drawer::Transform trans( width / 2, height / 2, 0, ( line * 32 ), 960, 540, WIDTH - width / 2, HEIGHT - height / 2 );
	//drawer->setSprite( Drawer::Sprite( trans, GRAPH_MATRIX ) );
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