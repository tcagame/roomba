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
#include "Model.h"
#include "Shadow.h"
#include "Delivery.h"

const int UI_PHASE_FOOT_X = 100;
const int UI_PHASE_Y = 40;
const int UI_NUM_SIZE = 64;
const int UI_MAP_SIZE = 9;
const int UI_MAP_X = 30;
const int UI_MAP_FOOT_Y = 30;
const int UI_MAP_RANGE = 15;
const int UI_NUM_SCROLL_TIME = 20;
const int UI_NUM_SCROLL_SPEED = 2;
const int FPS = 60;
const int CIRCLE_ANIME_FLAME = 1;
const int MAX_CHOICE_COUNT = 24 * CIRCLE_ANIME_FLAME;
const double GUIDELINE_VIEW_RANGE = 5 * WORLD_SCALE;
const int RESULT_TIME = 120;

SceneStage::SceneStage( int stage_num ) :
_choice_count( 0 ),
_tutorial_count( 0 ),
_crystal_catch_count( 0 ),
_result_count( 0 ) {
	_shadow = ShadowPtr( new Shadow );
	_guideline = ModelPtr( new Model );
	_viewer = ViewerPtr( new Viewer( _shadow ) );
	_timer = TimerPtr( new Timer );
	_roomba = RoombaPtr( new Roomba );
	_stage = StagePtr( new AppStage( stage_num, _viewer, _timer, _roomba ) );//0-2:通常 3:test_stage
	_camera = CameraPtr( new AppCamera( _roomba ) );
	
	_tutorial = false;
	if ( stage_num == 0 ) {
		_tutorial = true;
		DrawerPtr drawer = Drawer::getTask( );
		drawer->loadGraph( GRAPH_TUTORIAL_NEUTRAL, "tutorial/tutorial_neutral.png" );
		drawer->loadGraph( GRAPH_TUTORIAL_TRANSLATION, "tutorial/tutorial_translation.png" );
		drawer->loadGraph( GRAPH_TUTORIAL_ROTATION, "tutorial/tutorial_rotation.png" );
	}

	_delivery_number[ 0 ].state = NUMBER_STATE_IN;
	_delivery_number[ 1 ].state = NUMBER_STATE_NONE;
	_delivery_number[ 0 ].num = std::dynamic_pointer_cast<AppStage>( _stage )->getCarryCount( );
	_delivery_number[ 0 ].y = -UI_NUM_SCROLL_TIME * UI_NUM_SCROLL_SPEED;
	_phase_number[ 0 ].state = NUMBER_STATE_IN;
	_phase_number[ 1 ].state = NUMBER_STATE_NONE;


	_guideline->load( "../Resource/Model/Guideline/guideline.mdl" );
	_guideline->multiply( Matrix::makeTransformScaling( Vector( 0.3, 0.3, 0.3 ) ) );
	_guideline->multiply( Matrix::makeTransformRotation( Vector( 1, 0, 0 ), PI / 2 ) );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_SHADOW, "texture/shadow.png" );
	drawer->loadGraph( GRAPH_RADAR, "UI/radar.png" );
	drawer->loadGraph( GRAPH_COMMAND_BOOT, "UI/Boot.png" );
	drawer->loadGraph( GRAPH_COMMAND_REBOOT, "UI/ReBoot.png" );
	drawer->loadGraph( GRAPH_COMMAND_PROMPT_BACK, "UI/op_background.png" );
	drawer->loadGraph( GRAPH_LINK_GAUGE, "UI/link_gauge.png" );
	drawer->loadGraph( GRAPH_NUMBER, "UI/number.png" );
	drawer->loadGraph( GRAPH_PHASE, "UI/phase.png" );
	drawer->loadGraph( GRAPH_MAP, "UI/map.png" );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle.png" );
	drawer->loadGraph( GRAPH_GAME_OVER, "UI/game_over.png" );
	drawer->loadGraph( GRAPH_GAME_CLEAR, "UI/StageClear.png" );
	//model
	drawer->loadGraph( GRAPH_GUIDELINE, "Model/Guideline/guideline.jpg" );
	std::string floor_texture = "Model/Stage/floor_" + std::to_string( stage_num ) + ".jpg";
	drawer->loadGraph( GRAPH_FLOOR, floor_texture.c_str( ) );
	std::string wall_texture = "Model/Stage/wall_" + std::to_string( stage_num ) + ".jpg";
	drawer->loadGraph( GRAPH_WALL, wall_texture.c_str( ) );
	Matrix delivery_scale = Matrix::makeTransformScaling( DELIVERY_SIZE );

	Matrix crystal_scale = Matrix::makeTransformScaling( CRYSTAL_SIZE );
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", crystal_scale );
	
	drawer->loadMV1Model( MV1_BALL, "Model/Roomba/ball.mv1" );
	drawer->loadMV1Model( MV1_DELIVERY_STAND, "Model/Delivery/delivery_wait.mv1" );
	drawer->loadMV1Model( MV1_DELIVERY_CATCH, "Model/Delivery/delivery_catch.mv1" );
	drawer->loadMV1Model( MV1_DELIVERY_CARRY, "Model/Delivery/delivery_carry.mv1" );

	Matrix floor_scale = Matrix::makeTransformScaling( FLOOR_SIZE );
	drawer->loadMDLModel( MDL_FLOOR, "Model/Stage/_floor.mdl", "Model/Stage/colormap.png", floor_scale );

	drawer->loadEffect( EFFECT_CATCH_CRYSTAL, "Effect/catch.efk" );
	drawer->loadEffect( EFFECT_COLLISION_TO_WALL, "Effect/collision_wall.efk" );
	drawer->loadEffect( EFFECT_COLLISION_TO_CRYSTAL, "Effect/collision_crystal.efk" );
	drawer->loadEffect( EFFECT_CRYSTAL_AURA, "Effect/crystal_aura.efk" );
	drawer->loadEffect( EFFECT_DELIVERY_POINT, "Effect/point.efk" );
	drawer->loadEffect( EFFECT_ROOMBA, "Effect/move_roomba.efk" );
}


SceneStage::~SceneStage( ) {
}

Scene::NEXT SceneStage::update( ) {
	SoundPtr sound = Sound::getTask( );
	// カメラ&viwer常に更新する
	Roomba::MOVE_STATE roomba_move_state = _roomba->getMoveState( );
	if ( roomba_move_state != Roomba::MOVE_STATE_GAMEOVER ) {
		_camera->update( );
	}
	if ( roomba_move_state == Roomba::MOVE_STATE_STARTING ) {
		_viewer->update( _roomba->getStartPos( ) );
	} else {
		_viewer->update( _roomba->getCentralPos( ) );
		_tutorial_count++;
	}

	if ( _result_count > RESULT_TIME ) {
		_timer->finalize( );
		_roomba->finalize( );
		Game::getTask( )->setResult( roomba_move_state != Roomba::MOVE_STATE_GAMEOVER );
		if ( roomba_move_state != Roomba::MOVE_STATE_GAMEOVER ) {
			Game::getTask( )->setOpenStage( );
		}
		return NEXT_RESULT;
	}

	_roomba->update( _stage, _camera, _shadow, _timer );
	_stage->update( _camera );
	_roomba->updateLaser( _camera );

	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	if ( roomba_move_state != Roomba::MOVE_STATE_LIFT_UP &&
		 roomba_move_state != Roomba::MOVE_STATE_LIFT_DOWN &&
		 roomba_move_state != Roomba::MOVE_STATE_WAIT &&
		 roomba_move_state != Roomba::MOVE_STATE_STARTING &&
		 !app_stage->isFinished( ) ) {
		_timer->update( );
	}
	if ( roomba_move_state == Roomba::MOVE_STATE_WAIT ) {
		if ( _timer->getTime( ) < 5 * FPS ) {
			sound->stopSE( "alertSE.wav" );
		}
	}
	_stage->draw( );
	_shadow->update( );
	_shadow->draw( );
	_roomba->draw( );
	drawUI( );

	return Scene::NEXT_CONTINUE;
}

void SceneStage::drawUI( ) {
	drawUIDelivery( );
	drawUILinKGauge( );
	drawUIMap( );
	drawUIResult( );
	if ( _tutorial ) {
		if ( _roomba->isFirstCrystalCatch( ) ) {
			_crystal_catch_count++;
		}
		drawTutorial( );
	}
}

void SceneStage::drawUILinKGauge( ) {
	_timer->draw( );
}

void SceneStage::drawUIResult( ) {
	Roomba::MOVE_STATE roomba_move_state = _roomba->getMoveState( );
	if ( !_timer->isTimeOver( ) &&
		 !_roomba->isGameClear( ) ) {
		return;
	}
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WINDOW_WIDTH = app->getWindowWidth( );
	const int WINDOW_HEIGHT = app->getWindowHeight( );
	const int GRAPH_WIDTH = 1024;
	const int GRAPH_HEIGHT = 256;
	if ( _timer->isTimeOver( ) ) {
		double ratio =  _result_count * 0.07;
		if ( ratio > 1 ) {
			ratio = 1;
		}
		int sx = ( WINDOW_WIDTH - GRAPH_WIDTH ) / 2;
		int sy = ( WINDOW_HEIGHT - GRAPH_HEIGHT ) / 2;
		{//background
			Drawer::Transform trans( 0, 0, 0, 0, 512, 512, WINDOW_WIDTH, WINDOW_HEIGHT );
			Drawer::Sprite sprite( trans, GRAPH_COMMAND_PROMPT_BACK, Drawer::BLEND_ALPHA, ratio );
			drawer->setSprite( sprite );
		}
		{//gameover
			Drawer::Transform trans( sx, sy );
			Drawer::Sprite sprite( trans, GRAPH_GAME_OVER, Drawer::BLEND_ALPHA, ratio );
			drawer->setSprite( sprite );
		}
	}
	if ( _roomba->isGameClear( ) ) {//GAMECLEAR
		double ratio = _result_count;
		double size_ratio = 1 + _result_count * 0.02;
		if ( size_ratio > 1.2 && size_ratio < 1.4 ) {
			size_ratio = 1.2 - ( size_ratio - 1.2 );
		}
		if ( size_ratio >= 1.4 ) {
			size_ratio = 1;
		}
		int width =  ( int )( GRAPH_WIDTH  * size_ratio );
		int height = ( int )( GRAPH_HEIGHT * size_ratio );
		int sx1 = ( WINDOW_WIDTH  - width  ) / 2;
		int sy1 = ( WINDOW_HEIGHT - height ) / 2;
		int sx2 = sx1 + width;
		int sy2 = sy1 + height;
		Drawer::Transform trans( sx1, sy1, 0, 0, GRAPH_WIDTH, GRAPH_HEIGHT, sx2, sy2 );
		Drawer::Sprite sprite( trans, GRAPH_GAME_CLEAR );
		drawer->setSprite( sprite );
	}
	_result_count++;
}

void SceneStage::drawUIDelivery( ) {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	int scr_width = app->getWindowWidth( );

	int delivery_num = app_stage->getCarryCount( );
	
	int x = scr_width - UI_PHASE_FOOT_X;
	int y = UI_PHASE_Y;
	{
		int sx = x - UI_NUM_SIZE / 2;
		int sy = y - UI_NUM_SIZE / 3;
		int sx2 = sx + UI_NUM_SIZE * 2;
		int sy2 = sy + UI_NUM_SIZE * 2;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 0, 256, 256, sx2, sy2 ), GRAPH_PHASE ) );
	}
	
	if ( _delivery_number[ 0 ].num != delivery_num ) {
		_delivery_number[ 1 ] = _delivery_number[ 0 ];
		_delivery_number[ 1 ].state = NUMBER_STATE_OUT;
		_delivery_number[ 0 ].num = delivery_num;
		_delivery_number[ 0 ].state = NUMBER_STATE_IN;
		_delivery_number[ 0 ].y = -UI_NUM_SCROLL_TIME * UI_NUM_SCROLL_SPEED;
		_delivery_number[ 0 ].size = 0;
	}
	for ( int i = 0; i < 2; i++ ) {
		if ( _delivery_number[ i ].state == NUMBER_STATE_NONE ) {
			continue;
		}
		if ( _delivery_number[ i ].state == NUMBER_STATE_OUT ) {
			_delivery_number[ i ].y += UI_NUM_SCROLL_SPEED;
			_delivery_number[ i ].size -= 1.0 / UI_NUM_SCROLL_TIME;
			if ( _delivery_number[ i ].size < 0 ) {
				_delivery_number[ i ].size = 0;
			}
		}
		if ( _delivery_number[ i ].state == NUMBER_STATE_IN ) {
			_delivery_number[ i ].y += UI_NUM_SCROLL_SPEED;
			_delivery_number[ i ].size += 1.0 / UI_NUM_SCROLL_TIME;
			if ( _delivery_number[ i ].size > 1 ) {
				_delivery_number[ i ].size = 1;
				_delivery_number[ i ].state = NUMBER_STATE_WAIT;
			}
		}
		int number = _delivery_number[ i ].num;
		
		int max_digit = 0;
		if ( number / 10 ) {
			max_digit = 2;
		} else {
			max_digit = 1;
		}

		for ( int j = 0; j < max_digit; j++ ) {
			int sx = x + UI_NUM_SIZE / 2 - (int)( UI_NUM_SIZE * _delivery_number[ i ].size ) / 2 + ( j * ( UI_NUM_SIZE / 2 ) );
			if ( max_digit == 2 ) {
				sx -= UI_NUM_SIZE / 4;
			}
			int sy = y + UI_NUM_SIZE / 2 - (int)( UI_NUM_SIZE * _delivery_number[ i ].size ) / 2 + _delivery_number[ i ].y;
			int sx2 = sx + (int)( UI_NUM_SIZE * _delivery_number[ i ].size );
			int sy2 = sy + (int)( UI_NUM_SIZE * _delivery_number[ i ].size );

			int tx = 0;
			int digit = (int)pow( 10, max_digit - 1 - j );
			if ( digit > 9 ) {
				tx = ( number / digit ) * UI_NUM_SIZE;
			}
			if ( digit < 10 ) {
				tx = ( number % 10 ) * UI_NUM_SIZE;
			}

			Drawer::Transform trans( sx, sy, tx, 0, UI_NUM_SIZE, UI_NUM_SIZE, sx2, sy2 );
			drawer->setSprite( Drawer::Sprite( trans, GRAPH_NUMBER ) );
		}
	}
}


void SceneStage::drawUIMap( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	Vector base_pos = _roomba->getCentralPos( );
	base_pos.z = 0;
	Vector guideline_distance = Vector( 100, 100 );
	Vector dir = _camera->getDir( );
	dir.z = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( dir ), Vector( 0, -1 ).angle( dir ) );

	ApplicationPtr app = Application::getInstance( );
	int map_central_sx = UI_MAP_X + UI_MAP_SIZE * UI_MAP_RANGE;
	int map_central_sy = app->getWindowHeight( ) - UI_MAP_FOOT_Y - UI_MAP_SIZE * UI_MAP_RANGE;
	const int RANGE = UI_MAP_RANGE * UI_MAP_SIZE;
	{//背景
		int sx = map_central_sx - UI_MAP_SIZE * UI_MAP_RANGE;
		int sy = map_central_sy - UI_MAP_SIZE * UI_MAP_RANGE;
		int sx2 = sx + UI_MAP_SIZE * UI_MAP_RANGE * 2 + UI_MAP_SIZE;
		int sy2 = sy + UI_MAP_SIZE * UI_MAP_RANGE * 2 + UI_MAP_SIZE;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 256, 0, 256, 256, sx2, sy2 ), GRAPH_MAP ) );
		static int count = 0;
		count++;
		count %= 60;
		int tx = count % 5;
		int ty = count / 5;
		if ( ty > 4 ) {
			tx = 4;
			ty = 4;
		}
		const int TEX_SIZE = 128;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, tx * TEX_SIZE, ty * TEX_SIZE, TEX_SIZE, TEX_SIZE, sx2, sy2 ), GRAPH_RADAR ) );
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
	//デリバー表示
	std::list< DeliveryPtr > deliverys = app_stage->getDeliverys( );
	std::list< DeliveryPtr >::const_iterator delivery_ite = deliverys.begin( );
	while ( delivery_ite != deliverys.end( ) ) {
		DeliveryPtr delivery = *delivery_ite;
		if ( !delivery ) {
			delivery_ite++;
			continue;
		}
		if ( delivery->isHaveCrystal( ) ) {
			delivery_ite++;
			continue;
		}
		Vector pos = delivery->getPos( );
		pos.z = 0;
		Vector distance = ( getAdjustPos( pos, base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
		if ( !_roomba->isHoldCrystal( ) &&
			 ! _roomba->isFirstCrystalCatch( ) ) {
			if ( distance.getLength( ) < guideline_distance.getLength( ) ) {
				guideline_distance = distance;
			}
		}
		if ( distance.getLength( ) > RANGE ) {
			distance = distance.normalize( ) * RANGE;
		}
		distance = mat.multiply( distance );
		int sx = (int)( map_central_sx + distance.x );
		int sy = (int)( map_central_sy + distance.y );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 0, 32, 32, sx + UI_MAP_SIZE, sy + UI_MAP_SIZE ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.8 ) );
		delivery_ite++;
	}
	//クリスタル表示
	std::list< CrystalPtr > crystals = app_stage->getCrystals( );
	std::list< CrystalPtr >::const_iterator crystal_ite = crystals.begin( );
	while ( crystal_ite != crystals.end( ) ) {
		CrystalPtr crystal = *crystal_ite;
		if ( !crystal ) {
			crystal_ite++;
			continue;
		}
		Vector pos = crystal->getPos( );
		pos.z = 0;
		Vector distance = ( getAdjustPos( pos, base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
		if ( !_roomba->isHoldCrystal( ) &&
			 ! _roomba->isFirstCrystalCatch( ) ) {
			if ( distance.getLength( ) < guideline_distance.getLength( ) ) {
				guideline_distance = distance;
			}
		}
		if ( distance.getLength( ) > RANGE ) {
			distance = distance.normalize( ) * RANGE;
		}
		distance = mat.multiply( distance );
		int sx = (int)( map_central_sx + distance.x );
		int sy = (int)( map_central_sy + distance.y );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 32, 16, 16, sx + UI_MAP_SIZE, sy + UI_MAP_SIZE ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.8 ) );
		crystal_ite++;
	}
	// ガイドライン
	drawGuideLine( guideline_distance );
}

void SceneStage::drawTutorial( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	// コントローラー
	// トランスレーション
	if ( _roomba->getMoveState( ) != Roomba::MOVE_STATE_STARTING &&
		 !_roomba->isFirstCrystalCatch( ) &&
		 _tutorial_count < 120 ) {
		GRAPH graph = GRAPH_TUTORIAL_NEUTRAL;
		if ( _tutorial_count > 30 ) {
			graph = GRAPH_TUTORIAL_TRANSLATION;
		}
		double ratio = 1.0;
		if ( _tutorial_count < 15 ) {
			ratio = (double)_tutorial_count / 15;
		}
		if ( _tutorial_count > 150 ) {
			ratio = 1.0 - ( ( (double)_tutorial_count - 105 ) / 15 );
		}
		Drawer::Transform trans( ( WIDTH / 2 ) - 512, ( HEIGHT / 2 ) + 128, 0, 0, 1024, 256 );
		drawer->setSprite( Drawer::Sprite( trans, graph, Drawer::BLEND_ALPHA, ratio ) );

	}
	// ローテーション
	if ( _roomba->isFirstCrystalCatch( ) &&
		 _crystal_catch_count < 120 ) {
		GRAPH graph = GRAPH_TUTORIAL_ROTATION;
		double ratio = 1.0;
		if ( _crystal_catch_count < 15 ) {
			ratio = (double)_crystal_catch_count / 15;
		}
		if ( _crystal_catch_count > 120 ) {
			ratio = 1.0 - ( (double)( _crystal_catch_count - 105 ) / 15 );
		}
		Drawer::Transform trans( ( WIDTH / 2 ) - 512, ( HEIGHT / 2 ) + 128, 0, 0, 1024, 256 );
		drawer->setSprite( Drawer::Sprite( trans, graph, Drawer::BLEND_ALPHA, ratio ) );

	}	
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

void SceneStage::drawGuideLine( Vector distance ) const {
	Roomba::MOVE_STATE state = _roomba->getMoveState( );
	if ( state == Roomba::MOVE_STATE_LIFT_UP ||
		 state == Roomba::MOVE_STATE_LIFT_DOWN ||
		 state == Roomba::MOVE_STATE_WAIT ||
		 state == Roomba::MOVE_STATE_STARTING ||
		 distance == Vector( 100, 100 ) ) {
		return;
	}
	if ( distance.getLength( ) > GUIDELINE_VIEW_RANGE ) {
		ModelPtr guideline = ModelPtr( new Model );
		guideline->mergeModel( _guideline );
		Matrix rot = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( distance ) * -1, Vector( 0, -1 ).angle( distance ) );
		guideline->multiply( rot );
		guideline->setPos( _roomba->getCentralPos( ) + distance.normalize( ) + Vector( 0, 0, 1 ) );
		Drawer::ModelSelf self;
		self.model = guideline;
		self.graph = GRAPH_GUIDELINE;
		self.add = false;
		self.z_buffer = true;
		DrawerPtr drawer = Drawer::getTask( );
		drawer->setModelSelf( self );
	}
}

int SceneStage::getCrystalCarryNum( ) const {
	return std::dynamic_pointer_cast< AppStage >( _stage )->getCarryCount( );
}
