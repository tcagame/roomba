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

const int UI_PHASE_FOOT_X = 100;
const int UI_PHASE_Y = 40;
const int UI_NUM_SIZE = 64;
const int UI_MAP_SIZE = 6;
const int UI_MAP_X = 30;
const int UI_MAP_FOOT_Y = 30;
const int UI_MAP_RANGE = 20;
const int UI_NUM_SCROLL_TIME = 20;
const int UI_NUM_SCROLL_SPEED = 2;
const double GUIDELINE_VIEW_RANGE = 5 * WORLD_SCALE;

SceneStage::SceneStage( int stage_num ) {	
	_guideline = ModelPtr( new Model );
	_viewer = ViewerPtr( new Viewer );
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
	drawer->loadGraph( GRAPH_COMMAND_PROMPT_STRING, "UI/op.png" );
	drawer->loadGraph( GRAPH_COMMAND_PROMPT_BACK, "UI/op_background.png" );
	drawer->loadGraph( GRAPH_LINK_GAUGE, "UI/link_gauge.png" );
	drawer->loadGraph( GRAPH_NUMBER, "UI/number.png" );
	drawer->loadGraph( GRAPH_PHASE, "UI/phase.png" );
	drawer->loadGraph( GRAPH_MAP, "UI/map.png" );
	drawer->loadGraph( GRAPH_READY, "UI/ready.png" );	
	drawer->loadGraph( GRAPH_GUIDELINE, "Model/Guideline/guideline.jpg" );
	Matrix delivery_scale = Matrix::makeTransformScaling( DELIVERY_SIZE );

	Matrix crystal_scale = Matrix::makeTransformScaling( CRYSTAL_SIZE );
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", crystal_scale );
		
	Matrix roomba_scale = Matrix::makeTransformScaling( ROOMBA_SIZE );
	//drawer->loadMDLModel( MDL_BALL, "Model/Roomba/roomba.mdl", "Model/Roomba/texture.jpg", roomba_scale );
	
	Matrix earth_scale = Matrix::makeTransformScaling( EARTH_SIZE );
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", earth_scale );
	
	drawer->loadMV1Model( MV1_BALL, "Model/Roomba/roomba.mv1" );
	drawer->loadMV1Model( MV1_DELIVERY, "Model/Delivery/delivery.mv1" );

	drawer->loadMV1Model( MV1_WALL_0_1 , "Model/Stage/0_1.mv1"  );
	drawer->loadMV1Model( MV1_WALL_0_2 , "Model/Stage/0_2.mv1"  );
	drawer->loadMV1Model( MV1_WALL_0_3 , "Model/Stage/0_3.mv1"  );
	drawer->loadMV1Model( MV1_WALL_0_4 , "Model/Stage/0_4.mv1"  );

	drawer->loadMV1Model( MV1_WALL_0_5 , "Model/Stage/0_5.mv1"  );
	drawer->loadMV1Model( MV1_WALL_0_8 , "Model/Stage/0_8.mv1"  );
	drawer->loadMV1Model( MV1_WALL_0_10, "Model/Stage/0_10.mv1" );
	drawer->loadMV1Model( MV1_WALL_0_12, "Model/Stage/0_12.mv1" );
	drawer->loadMV1Model( MV1_WALL_0_15, "Model/Stage/0_15.mv1" );

	drawer->loadMV1Model( MV1_WALL_1_0 , "Model/Stage/1_0.mv1"  );
	drawer->loadMV1Model( MV1_WALL_1_1 , "Model/Stage/1_1.mv1"  );
	drawer->loadMV1Model( MV1_WALL_1_2 , "Model/Stage/1_2.mv1"  );
	drawer->loadMV1Model( MV1_WALL_1_3 , "Model/Stage/1_3.mv1"  );
	drawer->loadMV1Model( MV1_WALL_1_4 , "Model/Stage/1_4.mv1"  );
	drawer->loadMV1Model( MV1_WALL_1_5 , "Model/Stage/1_5.mv1"  );
	drawer->loadMV1Model( MV1_WALL_1_8 , "Model/Stage/1_8.mv1"  );
	drawer->loadMV1Model( MV1_WALL_1_10, "Model/Stage/1_10.mv1" );
	drawer->loadMV1Model( MV1_WALL_1_12, "Model/Stage/1_12.mv1" );
	drawer->loadMV1Model( MV1_WALL_1_15, "Model/Stage/1_15.mv1" );

	drawer->loadEffect( EFFECT_CATCH_CRYSTAL, "Effect/catch_crystal.efk" );
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
	// カメラ&viwer常に更新する
	_camera->update( );
	if ( _roomba->isStarting( ) ) {
		_viewer->update( _roomba->getStartPos( ) );
	} else {
		_viewer->update( _roomba->getCentralPos( ) );
	}

	if ( _timer->isTimeOver( ) ) {
		return NEXT_RETRY;
	}
	if ( _roomba->isFinished( ) ) {
		_timer->finalize( );
		return NEXT_RESULT;
	}

	_roomba->update( _stage, _camera );
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
	if ( _roomba->getMoveState( ) == Roomba::MOVE_STATE_LIFT_DOWN ) {
		_timer->reset( );
		app_stage->eraseOldDelivery( );
	}
	_roomba->draw( );
	_stage->draw( );
	drawUI( );

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
	}
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		//デリバー表示
		if ( _stage->getData( i ).delivery == app_stage->getDeliveryCount( ) ) {
			int x = i % STAGE_WIDTH_NUM;
			int y = i / STAGE_WIDTH_NUM;
			Vector station_pos( i % STAGE_WIDTH_NUM * WORLD_SCALE + WORLD_SCALE / 2, i / STAGE_WIDTH_NUM * WORLD_SCALE + WORLD_SCALE / 2 ); 
			Vector distance = ( getAdjustPos( station_pos, base_pos ) - base_pos ) * ( UI_MAP_SIZE / WORLD_SCALE );
			double length = distance.getLength( );
			if ( _roomba->getCrystalPtr( ) ) {
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
		if ( !_roomba->getCrystalPtr( ) ) {
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
	if ( _roomba->getMoveState( ) != Roomba::MOVE_STATE_LIFT_UP &&
		 _roomba->getMoveState( ) != Roomba::MOVE_STATE_LIFT_DOWN &&
		 _roomba->getMoveState( ) != Roomba::MOVE_STATE_WAIT &&
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
		drawer->setModelSelf( self );
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

