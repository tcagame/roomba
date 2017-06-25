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
const int CIRCLE_ANIME_FLAME = 3;
const int MAX_CHOICE_COUNT = 24 * CIRCLE_ANIME_FLAME;
const double GUIDELINE_VIEW_RANGE = 5 * WORLD_SCALE;
const double FADE_IN_RETRY_TIME = 30;

SceneStage::SceneStage( int stage_num ) :
_draw_count( 0 ),
_choice_count( 0 ),
_retry( true ) {
	_shadow = ShadowPtr( new Shadow );
	_guideline = ModelPtr( new Model );
	_viewer = ViewerPtr( new Viewer( _shadow ) );
	_timer = TimerPtr( new Timer );
	_roomba = RoombaPtr( new Roomba );
	_stage = StagePtr( new AppStage( stage_num, _viewer, _timer, _roomba ) );//0-2:通常 3:test_stage
	_camera = CameraPtr( new AppCamera( _roomba ) );
	

	_delivery_number[ 0 ].state = NUMBER_STATE_IN;
	_delivery_number[ 1 ].state = NUMBER_STATE_NONE;
	_delivery_number[ 0 ].num = _stage->getMaxDeliveryNum( ) - std::dynamic_pointer_cast<AppStage>( _stage )->getDeliveryCount( ) + 1;
	_delivery_number[ 0 ].y = -UI_NUM_SCROLL_TIME * UI_NUM_SCROLL_SPEED;
	_phase_number[ 0 ].state = NUMBER_STATE_IN;
	_phase_number[ 1 ].state = NUMBER_STATE_NONE;
	

	_guideline->load( "../Resource/Model/Guideline/guideline.mdl" );
	_guideline->multiply( Matrix::makeTransformScaling( Vector( 0.3, 0.3, 0.3 ) ) );
	_guideline->multiply( Matrix::makeTransformRotation( Vector( 1, 0, 0 ), PI / 2 ) );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_SHADOW, "texture/shadow.png" );
	drawer->loadGraph( GRAPH_RADAR, "UI/radar.png" );
	drawer->loadGraph( GRAPH_COMMAND_PROMPT_STRING, "UI/Boot.png" );
	drawer->loadGraph( GRAPH_COMMAND_PROMPT_BACK, "UI/op_background.png" );
	drawer->loadGraph( GRAPH_LINK_GAUGE, "UI/link_gauge.png" );
	drawer->loadGraph( GRAPH_NUMBER, "UI/number.png" );
	drawer->loadGraph( GRAPH_PHASE, "UI/phase.png" );
	drawer->loadGraph( GRAPH_MAP, "UI/map.png" );
	drawer->loadGraph( GRAPH_RETRY, "UI/retry.png" );
	drawer->loadGraph( GRAPH_RETRY_YES_NO, "UI/retry_select_yes_no.png" );
	drawer->loadGraph( GRAPH_FRAME, "UI/select_frame.png" );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle.png" );
	drawer->loadGraph( GRAPH_GUIDELINE, "Model/Guideline/guideline.jpg" );
	drawer->loadGraph( GRAPH_FLOOR, "Model/Stage/floor.jpg" );
	drawer->loadGraph( GRAPH_WALL, "Model/Stage/wall.jpg" );
	drawer->loadGraph( GRAPH_CONTROLLER_NEUTRAL, "controller/neutral.png" );
	drawer->loadGraph( GRAPH_CONTROLLER_TRANSLATION, "controller/translation.png" );
	drawer->loadGraph( GRAPH_CONTROLLER_ROTATION, "controller/rotation.png" );
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
	drawer->loadEffect( EFFECT_CHANGE_ROOMBA_STATE, "Effect/move_roomba.efk" );
	drawer->loadEffect( EFFECT_DELIVERY_POINT, "Effect/point.efk" );
	drawer->loadEffect( EFFECT_REBOOT, "Effect/reboot.efk" );

}


SceneStage::~SceneStage( ) {
}

Scene::NEXT SceneStage::update( ) {
	SoundPtr sound = Sound::getTask( );
	// カメラ&viwer常に更新する
	if ( _roomba->getMoveState( ) != Roomba::MOVE_STATE_GAMEOVER ) {
		_camera->update( );
	}
	if ( _roomba->getMoveState( ) == Roomba::MOVE_STATE_STARTING ) {
		_viewer->update( _roomba->getStartPos( ) );
	} else {
		_viewer->update( _roomba->getCentralPos( ) );
	}

	if ( _roomba->isFinished( ) && _roomba->getMoveState( ) != Roomba::MOVE_STATE_GAMEOVER ) {
		_timer->finalize( );
		_roomba->finalize( );
		sound->stopSE( "alertSE.wav" );
		return NEXT_RESULT;
	}

	_roomba->update( _stage, _camera, _shadow, _timer );
	_stage->update( _camera );
	_roomba->updateLaser( _camera );

	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	if ( _roomba->getMoveState( ) != Roomba::MOVE_STATE_LIFT_UP &&
		 _roomba->getMoveState( ) != Roomba::MOVE_STATE_LIFT_DOWN &&
		 _roomba->getMoveState( ) != Roomba::MOVE_STATE_WAIT &&
		 _roomba->getMoveState( ) != Roomba::MOVE_STATE_STARTING &&
		 !app_stage->isFinished( ) ) {
		_timer->update( );
	}
	if ( _roomba->getMoveState( ) == Roomba::MOVE_STATE_WAIT ) {
		if ( _timer->getTime( ) < 5 * FPS ) {
			sound->stopSE( "alertSE.wav" );
		}
		_timer->reset( );
	}
	_roomba->draw( );
	_stage->draw( );
	_shadow->update( );
	_shadow->draw( );
	drawUI( );

	if ( _roomba->isFinished( ) && _roomba->getMoveState( ) == Roomba::MOVE_STATE_GAMEOVER ) {
		drawRetry( );
		drawCircle( );
		return NextRetry( );
	}

	return Scene::NEXT_CONTINUE;
}

void SceneStage::drawUI( ) {
	drawUIDelivery( );
	drawUILinKGauge( );
	drawUIMap( );
}

void SceneStage::drawUILinKGauge( ) {
	_timer->draw( );
}

void SceneStage::drawUIDelivery( ) {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( _stage );
	int scr_width = app->getWindowWidth( );

	int delivery_num = _stage->getMaxDeliveryNum( ) - app_stage->getDeliveryCount( ) + 1;
	
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
		int sx = x  + UI_NUM_SIZE / 2 - (int)( UI_NUM_SIZE * _delivery_number[ i ].size ) / 2;
		int sy = y + UI_NUM_SIZE / 2  - (int)( UI_NUM_SIZE * _delivery_number[ i ].size ) / 2 + _delivery_number[ i ].y;
		int sx2 = sx + (int)( UI_NUM_SIZE * _delivery_number[ i ].size );
		int sy2 = sy + (int)( UI_NUM_SIZE * _delivery_number[ i ].size );
		Drawer::Sprite sprite( Drawer::Transform( sx, sy, number * UI_NUM_SIZE, 0, UI_NUM_SIZE, UI_NUM_SIZE, sx2, sy2 ), GRAPH_NUMBER );
		drawer->setSprite( sprite );
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
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		//デリバー表示
		if ( _stage->getData( i ).delivery == app_stage->getDeliveryCount( ) ) {
			int x = i % STAGE_WIDTH_NUM;
			int y = i / STAGE_WIDTH_NUM;
			Vector station_pos( i % STAGE_WIDTH_NUM * WORLD_SCALE + WORLD_SCALE / 2, i / STAGE_WIDTH_NUM * WORLD_SCALE + WORLD_SCALE / 2 ); 
			Vector distance = ( getAdjustPos( station_pos, base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
			double length = distance.getLength( );
			if ( _roomba->isHoldCrystal( ) ) {
				if ( length < guideline_distance.getLength( ) ) {
					guideline_distance = distance;
				}
			}
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
		Vector pos = crystal->getPos( );
		pos.z = 0;
		Vector distance = ( getAdjustPos( pos, base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
		if ( !_roomba->isHoldCrystal( ) ) {
			if ( distance.getLength( ) < guideline_distance.getLength( ) ) {
				guideline_distance = distance;
			}
		}
		distance = mat.multiply( distance );
		if ( distance.getLength( ) > RANGE ) {
			distance = distance.normalize( ) * RANGE;
		}
		int sx = (int)( map_central_sx + distance.x );
		int sy = (int)( map_central_sy + distance.y );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 32, 16, 16, sx + UI_MAP_SIZE, sy + UI_MAP_SIZE ), GRAPH_MAP, Drawer::BLEND_ALPHA, 0.8 ) );
		crystal_ite++;
	}
	// ガイドライン
	if ( _roomba->getMoveState( ) != Roomba::MOVE_STATE_LIFT_UP &&
		 _roomba->getMoveState( ) != Roomba::MOVE_STATE_LIFT_DOWN &&
		 _roomba->getMoveState( ) != Roomba::MOVE_STATE_WAIT &&
		 _roomba->getMoveState( ) != Roomba::MOVE_STATE_STARTING &&
		 guideline_distance.getLength( ) > GUIDELINE_VIEW_RANGE ) {
		ModelPtr guideline = ModelPtr( new Model );
		guideline->mergeModel( _guideline );
		Matrix rot = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( guideline_distance ) * -1, Vector( 0, -1 ).angle( guideline_distance ) );
		guideline->multiply( rot );
		guideline->setPos( base_pos + guideline_distance.normalize( ) + Vector( 0, 0, 1 ) );
		Drawer::ModelSelf self;
		self.model = guideline;
		self.graph = GRAPH_GUIDELINE;
		self.add = false;
		self.z_buffer = true;
		drawer->setModelSelf( self );
	}
}

void SceneStage::drawRetry( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	int WIDTH = app->getWindowWidth( );
	int HEIGHT = app->getWindowHeight( );
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 0, 0, 0, 0, 512, 512, WIDTH, HEIGHT ), GRAPH_COMMAND_PROMPT_BACK ) );

	const int TEXTURE_SIZE = 512;
	double RATIO = (double)( _draw_count ) / FADE_IN_RETRY_TIME;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( WIDTH / 2 - TEXTURE_SIZE / 2, HEIGHT / 2 - TEXTURE_SIZE / 2 ), GRAPH_RETRY, Drawer::BLEND_ALPHA, RATIO ) );

	const int SELECT_X = 192;
	const int SELECT_Y = 96;
	int sy = HEIGHT / 2 + SELECT_Y / 6;
	int sx = WIDTH / 2 - SELECT_X - 30;
	if ( !_retry ) {
		sx += SELECT_X + 60;
	}

	int flow = _draw_count % 20;
	if ( flow > 11 ) {
		flow = 20 - flow;
	}
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( sx - flow, sy - flow, 0, 0, SELECT_X, SELECT_Y, sx + SELECT_X + flow, sy + SELECT_Y + flow ), GRAPH_FRAME, Drawer::BLEND_ALPHA, RATIO ) ); // frame
}

void SceneStage::drawCircle( ) const {
	if ( _draw_count < FADE_IN_RETRY_TIME ) {
		return;
	}

	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	
	const int CIRCLE_SIZE = 100;
	int idx = _choice_count / CIRCLE_ANIME_FLAME;
	if ( idx > 24 || getFadeOutCount( ) != MAX_FADE_COUNT  ) {
		idx = 24;
	}
	int tx = idx % 5;
	int ty = idx / 5;
	int sy = HEIGHT / 2 - CIRCLE_SIZE / 2;
	if ( !_retry ) {
		sy += 128; // yes no の枠の大きさ分下にずらす
	}
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, sy, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	drawer->setSprite( sprite );
}	

void SceneStage::retry( ) {
	_roomba->retry( );
	_timer->reset( );
}

Scene::NEXT SceneStage::NextRetry( ) {
	_draw_count++;
	DevicePtr device = Device::getTask( );
	if ( device->getDirX( ) < 0 ) {
		_retry = true;
	}
	if ( device->getDirX( ) > 0 ) {
		_retry = false;
	}

	if ( device->getDirY( ) < 0 &&
		 device->getRightDirY( ) > 0 ) {
		_choice_count++;
		if ( _choice_count == 1 ) {
			SoundPtr sound = Sound::getTask( );
			sound->playSE( "circleSE.wav" );
		}
	} else {
		SoundPtr sound = Sound::getTask( );
		sound->stopSE( "circleSE.wav" );
		_choice_count = 0;
	}

	if ( _choice_count > MAX_CHOICE_COUNT ) {
		if ( _retry ) {
			retry( );
			_choice_count = 0;
			_draw_count = 0;
		} else {
			Sound::getTask( )->stopSE( "alertSE.wav" );
			return NEXT_GAMEOVER;
		}
	}
	return NEXT_CONTINUE;
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

