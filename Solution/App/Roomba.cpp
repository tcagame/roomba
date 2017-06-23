#include "Roomba.h"
#include "define.h"
#include "Ball.h"
#include "AppCamera.h"
#include "AppStage.h"
#include "Crystal.h"
#include "Device.h"
#include "laser.h"
#include <assert.h>
#include "Sound.h"
#include "Application.h"
#include "Game.h"
#include "Shadow.h"

//初速度
const double BOUND_POW = 0.7;
//加速度
const double ACCEL_SPEED = 0.03;
const double DELIVERY_ACCEL_SPEED = 0.03;
//一定速度
const double RESTORE_SPEED = 0.06;
//最大速度
const double MAX_ROT_SPEED = 0.2;
const double MAX_TRANS_SPEED = 0.4;
const double MAX_DELIVERY_SPEED = 0.3;
//減速速度
const double DECELETION_ROT_SPEED = 0.002;
const double DECELETION_TRANS_SPEED = 0.001;
const double DECELERATION_EMERGENCY_SPEED = 1;
const double DECELERATION_WAIT_SPEED = 0.2;
//減速倍率
const double OTHER_TRANS_RATIO = 6;
const double OTHER_ROT_RATIO = 2;
//サイズ
const double MAX_SCALE_SIZE = 6 + 0.01;
const double MIN_SCALE_SIZE = 5 - 0.01;
//ルンバ回収系
const double LIFT_Z = 10;
const double DELIVERY_FOOT = 1;
//エフェクト
const double EFFECT_REBOOT_SIZE = 0.7;
const double EFFECT_CHANGE_STATE_SIZE = 0.7;
//待機時間
const int WAIT_TIME = 180;
const int START_TIME = 162; // 随時要更新　ルンバがSTART_POSに配置されるまでのフレーム

const Vector START_POS[ 2 ] {//スケールが MIN < size < MAXになるようにする
	( Vector( STAGE_WIDTH_NUM + 19, STAGE_HEIGHT_NUM + 3 ) * WORLD_SCALE + Vector( 0, 0, BALL_RADIUS ) ),
	( Vector( STAGE_WIDTH_NUM + 22, STAGE_HEIGHT_NUM + 3 ) * WORLD_SCALE + Vector( 0, 0, BALL_RADIUS ) )
};

const Vector POP_POS[ 2 ] {
	Vector( -15, -5, 8 ),
	Vector( -18, -5, 8 )
};

Roomba::Roomba( ) :
_state( MOVE_STATE_STARTING ),
_trans_speed( Vector( ) ),
_rot_speed( 0 ),
_link_break( false ),
_finished( false ),
_wait_count( 0 ),
_start_count( 0 ) {
	for ( int i = 0; i < 2; i++ ) {
		_vec_trans[ i ] = Vector( );
		_balls[ i ] = BallPtr( new Ball( START_POS[ i ] + POP_POS[ i ] ) );
		_vec_rot[ i ] = Vector( );
		_vec_start[ i ] = Vector( );
		_vec_scale[ i ] = Vector( );
		_vec_reflection[ i ] = Vector( );
		_delivery[ i ] = AnimationPtr( new Animation( Animation::ANIM::ANIM_DELIVERY_CARRY ) );
	}
	for ( int i = 0; i < 4; i++ ) {	
		_boot[ 0 ][ i ] = true;
		_boot[ 1 ][ i ] = true;
	}
	_laser = LaserPtr( new Laser );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera, ShadowPtr shadow ) {
	changeState( stage, camera );
	updateState( );
	holdCrystal( stage );
	updateBalls( stage );
	shiftPos( camera );
	setShadow( shadow );
	updateDeliverys( );
}

void Roomba::setShadow( ShadowPtr shadow ) {
	const double SCALE = ( BALL_SIZE.x * SUN_POS ) / ( SUN_POS - BALL_SIZE.z );
	for ( int i = 0; i < 2; i++ ) {
		shadow->set( _balls[ i ]->getPos( ), SCALE );
	}
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	_laser->draw( );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->draw( );
		if ( _state == MOVE_STATE_LIFT_UP ||
			 _state == MOVE_STATE_LIFT_DOWN ||
			 _state == MOVE_STATE_STARTING ) {
			_delivery[ i ]->draw( );
		}
	}
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	if ( _state == MOVE_STATE_WAIT ) {
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( 0, 0, 0, 0, 512, 512, WIDTH, HEIGHT ), GRAPH_COMMAND_PROMPT_BACK ) );
	}
	drawCommandPrompt( );

	
	// チュートリアルコントローラー
	if ( _start_count > START_TIME &&
		 _start_count < (double)START_TIME * 3 ) {
		GRAPH graph = GRAPH_CONTROLLER_NEUTRAL;
		if ( _start_count > (double)START_TIME * 1.2 && 
			 _start_count < (double)START_TIME * 2 ) {
			graph = GRAPH_CONTROLLER_TRANSLATION;
		}
		if ( _start_count > (double)START_TIME * 2.2 ) {
			graph = GRAPH_CONTROLLER_ROTATION;
		}
		double ratio = 1.0;
		if ( _start_count < (double)START_TIME * 1.2 ) {
			ratio = (double)( _start_count - START_TIME ) / 10;
		}
		if ( _start_count > (double)START_TIME * 2.8 ) {
			ratio = 1.0 - ( (double)( _start_count - ( (double)START_TIME * 2.8 ) ) / 10 );
		}
		Drawer::Transform trans( ( WIDTH / 2 ) - 128, HEIGHT - 256, 0, 0, 512, 512, ( WIDTH / 2 ) + 128, HEIGHT );
		drawer->setSprite( Drawer::Sprite( trans, graph, Drawer::BLEND_ALPHA, ratio ) );
	}
}

void Roomba::drawCommandPrompt( ) const {
	if ( !_boot[ 0 ][ 0 ] && !_boot[ 1 ][ 0 ] ) {
		return;
	}

	if ( _start_count < START_TIME / 2 ) {
		drawPromptIn( );
	} else {
		drawPromptOut( );
	}
}

void Roomba::drawPromptIn( ) const {
	const int TW = 650;
	const int TH = 256;
	const int WAIT_TIME = 10;
	const int RATIO = 15;
	int draw_x = 10;
	if ( _start_count > WAIT_TIME ) {
		draw_x += ( _start_count - WAIT_TIME ) * RATIO;
		if ( draw_x > TW / 2 ) {
			draw_x = TW / 2;
		}
	}
	
	bool draw_str = false;
	int draw_y = 10;
	if ( draw_x == TW / 2 ) {
		draw_str = true;
		draw_y += ( _start_count - WAIT_TIME ) * RATIO - ( draw_x - 10 );
		if ( draw_y > TH / 2 ) {
			draw_y = TH / 2;
		}
	}
	ApplicationPtr app = Application::getInstance( );
	int sx = app->getWindowWidth( ) / 2;
	int sx2_left = sx - draw_x;
	if ( sx2_left < sx - TW / 2 ) {
		sx2_left = sx - TW / 2;
	}
	int sx2_right = sx + draw_x;
	if ( sx2_right > sx + TW / 2 ) {
		sx2_right = sx + TW / 2;
	}

	int sy = app->getWindowHeight( ) / 2 - draw_y;
	if ( sy < app->getWindowHeight( ) / 2 - TH / 2 ) {
		sy = app->getWindowHeight( ) / 2 - TH / 2;
	}
	int sy2 = app->getWindowHeight( ) / 2 + draw_y;
	if ( sy2 > app->getWindowHeight( ) / 2 + TH / 2 ) {
		sy2 = app->getWindowHeight( ) / 2 + TH / 2;
	}

	Drawer::getTask( )->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 0, 500, 500, sx2_left, sy2 ), GRAPH_COMMAND_PROMPT_BACK ) );
	Drawer::getTask( )->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 0, 500, 500, sx2_right, sy2 ), GRAPH_COMMAND_PROMPT_BACK ) );

	if ( draw_str ) {
		int tx_left = ( TW / 2 ) - draw_x;
		if ( tx_left < 0 ) {
			tx_left = 0;
		}
		int tw_left = ( TW / 2 ) - tx_left;
		Drawer::getTask( )->setSprite( Drawer::Sprite( Drawer::Transform( sx - tw_left, sy, tx_left, 0, tw_left, TH, sx, sy2 ), GRAPH_COMMAND_PROMPT_STRING ) );

		int tw_right = draw_x;
		if ( tw_right > TW / 2 ) {
			tw_right = TW / 2;
		}
		Drawer::getTask( )->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, TW / 2, 0, tw_right, TH, sx2_right, sy2 ), GRAPH_COMMAND_PROMPT_STRING ) );
	}
}

void Roomba::drawPromptOut( ) const {
	const int TW = 650;
	const int TH = 256;
	const int RATIO = 15;
	const int DRAW_COUNT = _start_count - ( START_TIME / 2 );
	int draw_x = ( TW  / 2 ) - ( DRAW_COUNT * RATIO );
	if ( draw_x < 10 ) {
		draw_x = 10;
	}
	
	int draw_y = TH / 2;
	if ( draw_x == 10 ) {
		draw_y -= ( DRAW_COUNT - ( ( TW / 2 ) / RATIO ) ) * RATIO;
		if ( draw_y < 10 ) {
			draw_y = 10;
		}
	}
	ApplicationPtr app = Application::getInstance( );
	int sx = app->getWindowWidth( ) / 2;
	int sx2_left = sx - draw_x;
	if ( sx2_left < sx - TW / 2 ) {
		sx2_left = sx - TW / 2;
	}
	int sx2_right = sx + draw_x;
	if ( sx2_right > sx + TW / 2 ) {
		sx2_right = sx + TW / 2;
	}

	int sy = app->getWindowHeight( ) / 2 - draw_y;
	if ( sy < app->getWindowHeight( ) / 2 - TH / 2 ) {
		sy = app->getWindowHeight( ) / 2 - TH / 2;
	}
	int sy2 = app->getWindowHeight( ) / 2 + draw_y;
	if ( sy2 > app->getWindowHeight( ) / 2 + TH / 2 ) {
		sy2 = app->getWindowHeight( ) / 2 + TH / 2;
	}

	Drawer::getTask( )->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 0, 500, 500, sx2_left, sy2 ), GRAPH_COMMAND_PROMPT_BACK ) );
	Drawer::getTask( )->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, 0, 0, 500, 500, sx2_right, sy2 ), GRAPH_COMMAND_PROMPT_BACK ) );

	if ( draw_x != 10 ) {
		int tx_left = ( TW / 2 ) - draw_x;
		if ( tx_left < 0 ) {
			tx_left = 0;
		}
		int tw_left = ( TW / 2 ) - tx_left;
		Drawer::getTask( )->setSprite( Drawer::Sprite( Drawer::Transform( sx - tw_left, sy, tx_left, 0, tw_left, TH, sx, sy2 ), GRAPH_COMMAND_PROMPT_STRING ) );

		int tw_right = draw_x;
		if ( tw_right > TW / 2 ) {
			tw_right = TW / 2;
		}
		Drawer::getTask( )->setSprite( Drawer::Sprite( Drawer::Transform( sx, sy, TW / 2, 0, tw_right, TH, sx2_right, sy2 ), GRAPH_COMMAND_PROMPT_STRING ) );
	}
}

void Roomba::updateState( ) {
	_wait_count--;
	_start_count++;
	acceleration( );
	moveTranslation( );
	moveRotation( );
	moveReflection( );
	moveRestore( );
	moveLiftUp( );
	moveLiftDown( );
	moveWait( );
	moveBound( );
	moveStarting( );
}

void Roomba::updateDeliverys( ) {
	for ( int i = 0; i < 2; i++ ) {
		_delivery[ i ]->update( );
		if ( _state == MOVE_STATE_STARTING ) {
			Vector ball = _balls[ i ]->getPos( );
			_delivery[ i ]->setPos( ball + Vector( 0, 0, DELIVERY_FOOT ) );
		} else {
			_delivery[ i ]->setPos( _delivery[ i ]->getPos( ) + Vector( 1, 1 ) * _vec_start[ i ].getLength( ) );
		}
	}
}

void Roomba::updateLaser( CameraConstPtr camera ) {
	bool show = !(
		_state == MOVE_STATE_LIFT_DOWN ||
		_state == MOVE_STATE_LIFT_UP ||
		_state == MOVE_STATE_REFLECTION ||
		_state == MOVE_STATE_REFLECTION_RESTORE ||
		_state == MOVE_STATE_STARTING );
	_laser->show( show );
	_laser->update( getCentralPos( ), camera, _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ), _crystal );
}

void Roomba::updateBalls( StagePtr stage ) {
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( stage );
	Vector vec[ 2 ];
	for ( int i = 0; i < 2; i++ ) {
		if ( _state == MOVE_STATE_REFLECTION ||
			 _state == MOVE_STATE_REFLECTION_RESTORE ) {
			vec[ i ] = _vec_reflection[ i ] + _vec_scale[ i ];
			vec[ i ].z = _vec_z[ i ];
		} else if ( _state == MOVE_STATE_LIFT_DOWN ) {
			vec[ i ].z = _vec_z[ i ];
		} else {
			vec[ i ] = _vec_trans[ i ] + _vec_rot[ i ];
			vec[ i ].z += _vec_z[ i ];
		}
	}

	for ( int i = 0; i < 2; i++ ) {
		bool rot = (
			_balls[ i ]->isReflection( ) ||
			( _state != MOVE_STATE_REFLECTION &&
			  _state != MOVE_STATE_LIFT_UP &&
			  _state != MOVE_STATE_LIFT_DOWN ) );
		_balls[ i ]->update( vec[ i ], stage, rot );
	}
}

void Roomba::changeState( StagePtr stage, CameraPtr camera ) {
	DevicePtr device = Device::getTask( );
	Vector right_stick = Vector( device->getRightDirX( ), device->getRightDirY( ) ) * 0.002;
	Vector left_stick = Vector( device->getDirX( ), device->getDirY( ) ) * 0.002;

	MOVE_STATE state = _state;
	_move_dir = Vector( );
	if ( right_stick.y > 0 && left_stick.y < 0 ) {
		state = MOVE_STATE_ROTATION_RIGHT;
		_move_dir = left_stick - right_stick;
	}
	if ( right_stick.y < 0 && left_stick.y > 0 ) {
		state = MOVE_STATE_ROTATION_LEFT;
		_move_dir = left_stick - right_stick;
	}
	if (  right_stick.y > 0 && left_stick.y > 0 ||
		  right_stick.y < 0 && left_stick.y < 0 ||
		  right_stick.x > 0 && left_stick.x > 0 ||
		  right_stick.x < 0 && left_stick.x < 0 ){
		state = MOVE_STATE_TRANSLATION;
		_move_dir = _stick_rot.multiply( right_stick + left_stick );
	}
	if ( right_stick == Vector( ) ||
		 left_stick == Vector( ) ) {
		state = MOVE_STATE_NEUTRAL;
	}
	if ( _state == MOVE_STATE_REFLECTION ) {
		if ( ( _vec_reflection[ 0 ] == Vector( ) )  &&
			 ( _vec_reflection[ 1 ] == Vector( ) ) ) {
			state = MOVE_STATE_REFLECTION_RESTORE;
		}
	}
	if ( _state == MOVE_STATE_REFLECTION_RESTORE ) {
		state = MOVE_STATE_REFLECTION_RESTORE;
		double scale = ( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) ).getLength( );	
		if ( scale > MIN_SCALE_SIZE && scale < MAX_SCALE_SIZE ) {
			state = MOVE_STATE_NEUTRAL;
		}
	}
	if ( ( _vec_reflection[ 0 ] != Vector( ) )  ||
		 ( _vec_reflection[ 1 ] != Vector( ) ) ) {
		state = MOVE_STATE_REFLECTION;
	}	
	
	for ( int i = 0; i < 2; i++ ) {
		if ( _balls[ i ]->isReflection( ) ) {
			state = MOVE_STATE_REFLECTION;
			_vec_reflection[ i ] = _balls[ i ]->getVec( );
			_vec_scale[ i ] = Vector( );
			_balls[ i ]->setReflection( false );
			if ( !( _vec_z[ i ] > 0 ) ) {
				_vec_z[ i ] = BOUND_POW;
			}
		}
	}
	
	if ( _wait_count > 0 ) {
		state = MOVE_STATE_WAIT;
	}
	if ( std::dynamic_pointer_cast< AppStage >( stage )->isFinished( ) ) {
		_link_break = true;
	}
	if ( _link_break ) {
		if ( !( _balls[ 0 ]->getPos( ).z > START_POS[ 0 ].z ) &&
			 !( _balls[ 1 ]->getPos( ).z > START_POS[ 1 ].z ) ) {
			state = MOVE_STATE_LIFT_UP;
		}
		if ( _state == MOVE_STATE_LIFT_UP ) {
			if ( _balls[ 0 ]->getPos( ).z < LIFT_Z ) {
				state = MOVE_STATE_LIFT_UP;
			} else {
				state = MOVE_STATE_LIFT_DOWN;
			}
		}
		if ( _state == MOVE_STATE_LIFT_DOWN ) {
			state = MOVE_STATE_LIFT_DOWN;
			SoundPtr sound = Sound::getTask( );
			//sound->playSE( "se_maoudamashii_effect14.wav" );
		}
	}
	if ( _boot[ 0 ][ 0 ] || _boot[ 1 ][ 0 ] ) {
		state = MOVE_STATE_STARTING;
	}

	if ( state != _state ) {
		//ステート変更がある場合の処理
		if ( state == MOVE_STATE_TRANSLATION ) {
			Vector dir = camera->getDir( );
			dir.z = 0;
			_stick_rot = Matrix::makeTransformRotation( Vector( 0, -1 ).cross( dir ) * -1, Vector( 0, -1 ).angle( dir ) );
			_move_dir = _stick_rot.multiply( right_stick + left_stick );
		}
		if ( state == MOVE_STATE_REFLECTION ) {
			SoundPtr sound = Sound::getTask( );
			sound->playSE( "knocking_a_wall.mp3" );
			_trans_speed = Vector( );
			_rot_speed = 0;
		}
		if ( state == MOVE_STATE_LIFT_UP ) {
			_delivery[ 0 ]->setPos( Vector( -1, -1 ) );
			setVecReflection( Vector( ), Vector( ) );
			setVecScale( Vector( ), Vector( ) );
			_crystal = CrystalPtr( );
		}
		if ( state == MOVE_STATE_WAIT ) {
			setVecTrans( Vector( ) );
			setVecRot( Vector( ), Vector( ) );
		}
		if ( _state == MOVE_STATE_LIFT_UP ) {
			_vec_z[ 0 ] = 0;
			_vec_z[ 1 ] = 0;
			for ( int i = 0; i < 2; i++ ) {
				Vector ball = START_POS[ i ] + Vector( 0, 0, LIFT_Z );
				_balls[ i ]->setPos( ball );
				_delivery[ i ]->setPos( ball + Vector( 0, 0, DELIVERY_FOOT ) );
			}
			if ( std::dynamic_pointer_cast< AppStage >( stage )->isFinished( ) ) {
				_finished = true;
			}
		}
		announceChangeState( state );
		_state = state;
	}
}

void Roomba::acceleration( ) {
	switch ( _state ) {
	case MOVE_STATE_TRANSLATION:
		brakeRotation( );
		accelTranslation( );
		break;
	case MOVE_STATE_ROTATION_RIGHT:
		brakeTranslation( );
		accelRotation( DIR_RIGHT );
		break;
	case MOVE_STATE_ROTATION_LEFT:
		brakeTranslation( );
		accelRotation( DIR_LEFT );
		break;
	case MOVE_STATE_NEUTRAL:
	case MOVE_STATE_LIFT_UP:
		brakeTranslation( );
		brakeRotation( );
		break;
	case MOVE_STATE_REFLECTION:
	case MOVE_STATE_REFLECTION_RESTORE:
	case MOVE_STATE_WAIT:
		break;
		break;
	default:
		assert( -1 );
		break;
	}
}

void Roomba::accelTranslation( ) {
	_trans_speed += _move_dir * ACCEL_SPEED;
	if ( _trans_speed.getLength( ) > MAX_TRANS_SPEED ) {
		_trans_speed = _trans_speed.normalize( ) * MAX_TRANS_SPEED;
	}
}

void Roomba::accelRotation( DIR dir ) {
	_rot_speed += _move_dir.y * ACCEL_SPEED;
	if ( fabs( _rot_speed ) > MAX_ROT_SPEED ) {
		_rot_speed = MAX_ROT_SPEED * ( 1 - ( _rot_speed < 0 ) * 2 );
	}
}

void Roomba::brakeTranslation( ) {
	double deceletion_trans_speed = DECELETION_TRANS_SPEED;
	if ( _state != MOVE_STATE_TRANSLATION ) {
		deceletion_trans_speed *= OTHER_TRANS_RATIO;
	}
	if ( _state == MOVE_STATE_LIFT_UP ) {
		deceletion_trans_speed *= 3;
	}
	if ( _move_dir.getLength( ) > 0.001 ) {
		deceletion_trans_speed = ACCEL_SPEED / 2;
	}
	if ( _trans_speed.getLength( ) > deceletion_trans_speed ) {
		_trans_speed -= _trans_speed.normalize( ) * deceletion_trans_speed;
	} else {
		_trans_speed = Vector( );
	}
}

void Roomba::brakeRotation( ) {
	double deceletion_speed = DECELETION_ROT_SPEED;
	if ( _state != MOVE_STATE_ROTATION_LEFT &&
		 _state != MOVE_STATE_ROTATION_RIGHT ) {
		deceletion_speed *= OTHER_ROT_RATIO;
	}
	if ( _state == MOVE_STATE_LIFT_UP ) {
		deceletion_speed *= 3;
	}
	if ( fabs( _rot_speed ) > deceletion_speed ) {
		_rot_speed += deceletion_speed * ( 1 - ( _rot_speed > 0 ) * 2 );
	 } else {
		_rot_speed = 0;
	}
}

void Roomba::holdCrystal( StagePtr stage ) {
	if ( !_crystal ) {
		_crystal = std::dynamic_pointer_cast< AppStage >( stage )->getHittingCrystal( _balls[ 0 ]->getPos( ), _balls[ 1 ]->getPos( ), _balls[ 0 ]->getVec( ), _balls[ 1 ]->getVec( ) );
		if ( _crystal ) {
			_crystal->setDropDown( false );
		}
	}
	if ( _crystal ) {
		if ( _crystal->isDropDown( ) ||
			 _crystal->isFinished( ) ||
			_state == MOVE_STATE_REFLECTION ||
			_state == MOVE_STATE_REFLECTION_RESTORE ||
			_state == MOVE_STATE_STARTING ) {
			_crystal->setDropDown( true );
			_crystal = CrystalPtr( );
			return;
		}
		Vector crystal_pos = _crystal->getPos( );
		Vector pos[ 2 ] = {
			_balls[ 0 ]->getPos( ) + _balls[ 0 ]->getVec( ),
			_balls[ 1 ]->getPos( ) + _balls[ 1 ]->getVec( )
		};
		pos[ 0 ].z = START_POS[ 0 ].z;
		pos[ 1 ].z = START_POS[ 1 ].z;
		crystal_pos.z = START_POS[ 0 ].z;
		Vector line = pos[ 1 ] - pos[ 0 ];
		pos[ 0 ] += line.normalize( ) * BALL_RADIUS * 3;
		pos[ 1 ] -= line.normalize( ) * BALL_RADIUS * 3;
		line = pos[ 1 ] - pos[ 0 ];
		Vector to_crystal = crystal_pos - pos[ 0 ];
		double angle = line.angle( to_crystal );
		double cos_length = to_crystal.getLength( ) * cos( angle );
		Vector vec;
		if ( cos_length < 0 ) {							//線より外側
			vec = pos[ 0 ] - crystal_pos;
		} else if ( cos_length > line.getLength( ) ) {	//線より外側
			vec = pos[ 1 ] - crystal_pos;
		} else {									//線に垂直
			double sin_length = to_crystal.getLength( ) * fabs( sin( angle ) );
			Matrix rot = Matrix::makeTransformRotation( line.cross( to_crystal ), PI / 2 );
			vec = rot.multiply( line ).normalize( ) * sin_length;
			if ( vec.getLength( ) < 0.1 ) {
				vec = vec * ( vec.getLength( ) * 10 );
			}
		}
		Vector to_central = line.normalize( ) * ( line.getLength( ) / 2 - cos_length ) * 0.1;
		vec += to_central;
		_crystal->setVec( vec );
	}
}

void Roomba::moveTranslation( ) {
	setVecTrans( _trans_speed );
}

void Roomba::moveRotation( ) {
	std::array< Vector, 2 > vec;
	for ( int i = 0;  i < 2; i++ ) {
		Vector radius = _balls[ i ]->getPos( ) - getCentralPos( );
		Matrix mat_rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), _rot_speed / radius.getLength( ) );
		Vector radius2 = mat_rot.multiply( radius );
		vec[ i ] = ( ( radius2 + getCentralPos( ) ) - _balls[ i ]->getPos( ) );
	}
	setVecRot( vec[ 0 ], vec[ 1 ] );
}

void Roomba::moveReflection( ) {
	if ( _state != MOVE_STATE_REFLECTION ) {
		return;
	}
	for ( int i = 0; i < 2; i++ ) {
		Vector pos = _balls[ i ]->getPos( );
		if ( pos.z + _vec_z[ i ] > START_POS[ i ].z ) {
			continue;
		}
		if ( _vec_reflection[ i ].getLength( ) > DECELERATION_EMERGENCY_SPEED ) {
			_vec_reflection[ i ] -= _vec_reflection[ i ].normalize( ) * DECELERATION_EMERGENCY_SPEED;
		} else {
			_vec_reflection[ i ] = Vector( );
		}
	}
}

void Roomba::moveRestore( ) {
	if ( _state != MOVE_STATE_REFLECTION_RESTORE ) {
		setVecScale( Vector( ), Vector( ) );
		return;
	}

	Vector laser = _balls[ 1 ]->getPos( ) - _balls[ 0 ]->getPos( );
	laser.z = 0;
	double scale = ( _balls[ 1 ]->getPos( ) - _balls[ 0 ]->getPos( ) ).getLength( );
	for ( int i = 0; i < 2; i++ ) {
		Vector pos = _balls[ i ]->getPos( );
		if ( pos.z + _vec_z[ i ] > START_POS[ i ].z ) {
			continue;
		}
		if ( scale > MAX_SCALE_SIZE ) {
				_vec_scale[ i ] = ( laser * ( scale - MAX_SCALE_SIZE ) * ( 1 - ( i != 0 ) * 2 ) ).normalize( ) * RESTORE_SPEED;
		} else  {
			if ( scale < MIN_SCALE_SIZE ) {
				_vec_scale[ i ] = ( laser * ( MIN_SCALE_SIZE - scale ) * ( 1 - ( i != 0 ) * 2 ) ).normalize( ) * -RESTORE_SPEED;
			} else {
				_vec_scale[ i ] = Vector( );
			}
		}
	}

}

void Roomba::moveLiftUp( ) {
	if ( _state != MOVE_STATE_LIFT_UP ) {
		return;
	}
	Stage::MV1_INFO delivery[ 2 ] = {
		_delivery[ 0 ]->getModel( ),
		_delivery[ 1 ]->getModel( ),
	};
	if ( _trans_speed.getLength( ) > 0 ||
		 _rot_speed > 0 ) {
		_vec_z[ 0 ] = 0;
		_vec_z[ 1 ] = 0;
		return;
	} else {
		if ( delivery[ 0 ].pos.x < 0 ) {
			delivery[ 0 ].pos = _balls[ 0 ]->getPos( ) + Vector( 0, 0, LIFT_Z );
			delivery[ 1 ].pos = _balls[ 1 ]->getPos( ) + Vector( 0, 0, LIFT_Z );
		}
	}

	for ( int i = 0; i < 2; i++ ) {
		Vector ball = _balls[ i ]->getPos( ) + Vector( 0, 0, DELIVERY_FOOT );
		Vector distance = delivery[ i ].pos - ball;
		if ( distance.z > -0.0001 ) {
			delivery[ i ].pos -= distance.normalize( ) * 0.1;
			_vec_z[ i ] = 0;
		} else {
			delivery[ i ].pos.z = ball.z;
			_vec_z[ i ] = 0.2;
		}
		_delivery[ i ]->setPos( delivery[ i ].pos );
	}
}

void Roomba::moveLiftDown( ) {
	if ( _state != MOVE_STATE_LIFT_DOWN ) {
		return;
	}
	for ( int i = 0; i < 2; i++ ) {
		Vector pos = _balls[ i ]->getPos( );
		Vector vec = START_POS[ i ] - pos;
		if ( fabs( pos.z - START_POS[ i ].z ) < 0.01 && fabs( _vec_z[ i ] ) < 0.01 ) {
			_link_break = false;
		}
	}
}

void Roomba::moveBound( ) {
	if ( _state == MOVE_STATE_LIFT_UP ) {
		return;
	}
	for ( int i = 0; i < 2; i++ ) {
		Vector pos = _balls[ i ]->getPos( );
		if ( pos.z > START_POS[ i ].z ) {
			_vec_z[ i ] -= GRAVITY;
			if ( pos.z + _vec_z[ i ] < START_POS[ i ].z ) {
				if ( fabs( _vec_z[ i ] ) > GRAVITY * 2 ) {
					_vec_z[ i ] *= -0.3;
				} else {
					_vec_z[ i ] = START_POS[ i ].z - pos.z;
				}
			}
		}
		if ( pos.z == START_POS[ i ].z ) {
			if ( _vec_z[ i ] < 0 ) {
				_vec_z[ i ] = 0;
			}
		}
	}
}

void Roomba::moveStarting( ) {
	if ( _state != MOVE_STATE_STARTING ) {
		return;
	}
	setVecReflection( Vector( ), Vector( ) );
	setVecRot( Vector( ), Vector( ) );
	setVecScale( Vector( ), Vector( ) );
	setVecTrans( Vector( ) );

	Vector target[ 2 ];
	for ( int i = 0; i < 2; i++ ) {
		const Vector TARGET[ 4 ] = {
			START_POS[ i ] + Vector(  0,  0 ) * WORLD_SCALE,
			START_POS[ i ] + Vector(  1,  0 ) * WORLD_SCALE,
			START_POS[ i ] + Vector( -4, 1 ) * WORLD_SCALE,
			START_POS[ i ] + Vector(  4,  0 ) * WORLD_SCALE
		};
		for ( int j = 0; j < 4; j++ ) {
			if ( _boot[ i ][ j ] ) {
				target[ i ] = TARGET[ j ];
			}
		}
	}

	Vector pos[ 2 ] = {
		_balls[ 0 ]->getPos( ),
		_balls[ 1 ]->getPos( )
	};
	Vector vec2 = Vector( sin( _start_count * PI / 150 ), sin( _start_count * PI / 100 ) ) * 0.01;
	for ( int i = 0; i < 2; i++ ) {
		if ( !_boot[ i ][ 0 ] ) {
			continue;
		}
		Vector diff;
		diff = target[ i ] - pos[ i ];
		diff.z = 0;
		double accel = DELIVERY_ACCEL_SPEED;
		if ( _vec_start[ i ].angle( diff ) > PI / 4 ) {
			accel *= 1.2;
		}
		_vec_start[ i ] += diff.normalize( ) * accel + vec2;
		if ( _vec_start[ i ].getLength( ) > MAX_DELIVERY_SPEED ) {
			_vec_start[ i ] = _vec_start[ i ].normalize( ) * MAX_DELIVERY_SPEED;
		}
		_vec_z[ i ] = 0;

		if ( diff.getLength( ) < 0.4 ) {
			for ( int j = 3; j > 0; j-- ) {
				if ( _boot[ i ][ j ] ) {
					_boot[ i ][ j ] = false;
					break;
				}
			}
		}
		if ( _boot[ i ][ 0 ] && !_boot[ i ][ 1 ] ) {
			if ( diff.getLength( ) < 1.5 ) {
				_vec_start[ i ] *= 0.7;
			}
			if ( ( _vec_start[ i ].angle( diff ) < PI / 100 ) ||
				 ( _vec_start[ i ].getLength( ) < 0.01 && diff.getLength( ) < 0.1 ) ) {
				_vec_start[ i ] = Vector( );
				_boot[ i ][ 0 ] = false;
				_delivery[ i ]->changeAnim( Animation::ANIM::ANIM_DELIVERY_DISENGAGE );
			}
		}
	}

	setVecRot( _vec_start[ 0 ], _vec_start[ 1 ] );
	
}

void Roomba::moveWait( ) {
	if ( _state != MOVE_STATE_WAIT ) {
		return;
	}
	for ( int i = 0; i < 2; i++ ) {
		Vector pos = _balls[ i ]->getPos( );
		Vector vec = _balls[ i ]->getVec( );
		vec.z = 0;
		if ( pos.z + _vec_z[ i ] > START_POS[ i ].z ) {
			_vec_trans[ i ] = vec;
			continue;
		}
		if ( vec.getLength( ) > DECELERATION_WAIT_SPEED ) {
			vec -= vec.normalize( ) * DECELERATION_WAIT_SPEED;
		} else {
			vec = Vector( );
		}
		_vec_trans[ i ] = vec;
	}
}

Vector Roomba::getCentralPos( ) const {
	Vector pos[ 2 ];
	for ( int i = 0; i < 2; i++ ) {
		pos[ i ] = _balls[ i ]->getPos( );
	}
	Vector central_pos = ( pos[ 0 ] + pos[ 1 ] ) * 0.5;

	return central_pos;
}

void Roomba::reset( ) {
	_balls[ BALL_LEFT ]->reset( START_POS[ 0 ] );
	_balls[ BALL_RIGHT ]->reset( START_POS[ 1 ] );
	_trans_speed = Vector( );
	_rot_speed = 0;
	for ( int i = 0; i < 2; i++ ) {
		_vec_trans[ i ] = Vector( );
		_vec_rot[ i ] = Vector( );
		_vec_scale[ i ] = Vector( );
		_vec_reflection[ i ] = Vector( );
	}
	_crystal = CrystalPtr( );
}

void Roomba::setWaitCount( ) {
	_wait_count = WAIT_TIME;
}

void Roomba::checkLeftRight( CameraPtr camera ) {	
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector vec = mat.multiply( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) );
	double dot = camera->getDir( ).dot( vec );
	if ( dot > 0 ) {
		BallPtr tmp = _balls[ BALL_LEFT ];
		_balls[ BALL_LEFT ] = _balls[ BALL_RIGHT ];
		_balls[ BALL_RIGHT ] = tmp;
	}
}

Vector Roomba::getBallPos( int ball ) const {
	return _balls[ ball ]->getPos( );
}

Roomba::MOVE_STATE Roomba::getMoveState( ) const {
	return _state;
}

bool Roomba::isWait( ) const {
	return ( _wait_count > 0 );
}

bool Roomba::isFinished( ) const {
	return _finished;
}

Vector Roomba::getStartPos( ) const {
	return ( START_POS[ 0 ] + START_POS[ 1 ] ) * 0.5;
}

bool Roomba::isHoldCrystal( ) const {
	return ( _crystal != CrystalPtr( ) );
}

void Roomba::setVecTrans( Vector& vec ) {
	_vec_trans[ 0 ] = vec;
	_vec_trans[ 1 ] = vec;
}

void Roomba::setVecRot( Vector& vec_left, Vector& vec_right ) {
	_vec_rot[ 0 ] = vec_left;
	_vec_rot[ 1 ] = vec_right;
}

void Roomba::setVecScale( Vector& vec_left, Vector& vec_right ) {
	_vec_scale[ 0 ] = vec_left;
	_vec_scale[ 1 ] = vec_right;
}

void Roomba::setVecReflection( Vector& vec_left, Vector& vec_right ) {
	_vec_reflection[ 0 ] = vec_left;
	_vec_reflection[ 1 ] = vec_right;
}

void Roomba::shiftPos( CameraPtr camera ) {
	Vector pos[ 2 ];
	pos[ 0 ] = _balls[ 0 ]->getPos( );
	pos[ 1 ] = _balls[ 1 ]->getPos( );
	Vector central_pos =  getCentralPos( );
	bool shift = false;
	if ( central_pos.x < STAGE_WIDTH_NUM * WORLD_SCALE - 1 ) {
		shift = true;
		central_pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].x += STAGE_WIDTH_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
	if ( central_pos.y < STAGE_HEIGHT_NUM * WORLD_SCALE - 1 ) {
		shift = true;
		central_pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
		for ( int i = 0; i < 2; i++ ) {
			pos[ i ].y += STAGE_HEIGHT_NUM * WORLD_SCALE;
			_balls[ i ]->setPos( pos[ i ] );
		}
	}
	AppCameraPtr app_camera = std::dynamic_pointer_cast< AppCamera >( camera );
	if ( shift ) {
		if ( _crystal ) {
			_crystal->shiftPos( central_pos );
		}
		app_camera->shiftPos( central_pos );
	}
}

void Roomba::announceChangeState( MOVE_STATE state ) {
	if ( _state == MOVE_STATE_REFLECTION_RESTORE &&
		 state != MOVE_STATE_REFLECTION_RESTORE ) {
		Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_REBOOT, getCentralPos( ), EFFECT_REBOOT_SIZE, EFFECT_ROTATE ) );
		return;
	}
	if ( state == MOVE_STATE_REFLECTION ||
		 state == MOVE_STATE_REFLECTION_RESTORE ||
		 state == MOVE_STATE_NEUTRAL ||
		 state == MOVE_STATE_LIFT_UP ||
		 state == MOVE_STATE_LIFT_DOWN ) {
		return;
	}
	if ( ( _state == MOVE_STATE_ROTATION_RIGHT || _state == MOVE_STATE_ROTATION_LEFT ) &&
		 ( state == MOVE_STATE_ROTATION_RIGHT || state == MOVE_STATE_ROTATION_LEFT ) ) {
		return;
	}

	Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_CHANGE_ROOMBA_STATE, getCentralPos( ), EFFECT_CHANGE_STATE_SIZE, EFFECT_ROTATE ) );
}

Vector Roomba::getDir( ) const {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector roomba_dir = mat.multiply( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) );
	return roomba_dir.normalize( );
}

double Roomba::getRotSpeed( ) const {
	return _rot_speed;
}

const double Roomba::getMaxSpeed( ) const {
	return MAX_TRANS_SPEED;
}

void Roomba::finalize( ) {
	int col_num = _balls[ BALL_LEFT ]->getColCount( ) + _balls[ BALL_RIGHT ]->getColCount( );
	Game::getTask( )->setCollisionNum( col_num );	
}