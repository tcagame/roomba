#include "SceneResult.h"
#include "Drawer.h"
#include "Device.h"
#include "define.h"
#include "Game.h"
#include "Application.h"
#include "Binary.h"
#include "Sound.h"

const int RESULT_WIDTH  = 512;
const int RESULT_HEIGHT = 256;
const int THICK_FRAME_SIZE = 57;
const int SELECT_SIZE  = 512;
const int CIRCLE_ANIME_FLAME = 1;
const int MAX_CHOICE_COUNT = 25 * CIRCLE_ANIME_FLAME;
const int SELECT_TEXTURE_X = 512;
const int SELECT_TEXTURE_Y = 128;
const double SELECT_SMALL_SIZE = 0.75;

SceneResult::SceneResult( int crystal_carry_num ) :
_choice_count( 0 ),
_count( 0 ),
_crystal_carry_num( crystal_carry_num ),
_move_count( 0 ),
_retry( true ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_NUMBER, "UI/number.png" );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle.png" );
	drawer->loadGraph( GRAPH_STAGE_SELECT, "select/Stage Select.png" );
	drawer->loadGraph( GRAPH_GAME_OVER, "UI/result_str.png" );
	drawer->loadGraph( GRAPH_RESULT_FRAME, "UI/game_over_frame.png" );
	drawer->loadGraph( GRAPH_RESULT, "UI/result.png" );
	
	drawer->loadGraph( GRAPH_RETRY, "UI/retry.png" );
	drawer->loadGraph( GRAPH_YES_NO, "UI/yes_no.png" );
		
	const int WIDTH = Application::getInstance( )->getWindowWidth( );
	const int HEIGHT = Application::getInstance( )->getWindowHeight( );
	_select_pos[ 0 ] = Vector( WIDTH / 2 - SELECT_TEXTURE_X / 4, HEIGHT * 6 / 8, 1 );
	_select_pos[ 1 ] = Vector( WIDTH / 2 - SELECT_TEXTURE_X / 4 + SELECT_TEXTURE_X * 2 / 3, HEIGHT * 6 / 8, SELECT_SMALL_SIZE );

	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "resultBGM.wav" );
}

SceneResult::~SceneResult( ) {
}

Scene::NEXT SceneResult::update( ) {
	_count++;
	SoundPtr sound = Sound::getTask( );
	DevicePtr device = Device::getTask( );
	Vector right_stick = Vector( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick = Vector( device->getDirX( ), device->getDirY( ) );

	draw( );

	// フェードイン
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		addFadeInCount( );
		return NEXT_CONTINUE;
	}

	// フェードアウト
	if ( _choice_count > MAX_CHOICE_COUNT ||
		getFadeOutCount( ) < MAX_FADE_COUNT ) {
		subFadeOutCount( );
		if ( getFadeOutCount( ) < 0 ) {
			if ( _retry ) {
				return NEXT_STAGE;
			} else {
				return NEXT_TITLE;
			}
		}
	}
	
	selectRetry( );

	return NEXT_CONTINUE;
}

void SceneResult::draw( ) const {
	drawFadeBg( );
	drawFrame( );
	drawResult( );
	drawCrystalNum( );
	drawRetry( );
	drawCircle( );
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		drawFadeIn( );
	} else {
		drawFadeOut( );
	}	
}

void SceneResult::drawResult( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int RESULT_WIDTH = 180;
	const int RESULT_HEIGHT = 50;

	Drawer::Transform trans( WIDTH / 2 - RESULT_WIDTH / 2, RESULT_HEIGHT * 5 );
	Drawer::Sprite sprite( trans, GRAPH_GAME_OVER );
	drawer->setSprite( sprite );
}

void SceneResult::drawRetry( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	int WIDTH = app->getWindowWidth( );
	int HEIGHT = app->getWindowHeight( );

	const int MAX_FLOW = 20;
	const int TEXTURE_X = SELECT_TEXTURE_X;
	const int TEXTURE_Y = SELECT_TEXTURE_Y;
	int retry_sx = WIDTH / 2 - TEXTURE_X * 2 / 6;
	int retry_sy = HEIGHT / 2 + TEXTURE_Y;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( retry_sx, retry_sy, 0, 0, TEXTURE_X, TEXTURE_Y, retry_sx + TEXTURE_X * 2 / 3, retry_sy + TEXTURE_Y * 2 / 3 ), GRAPH_RETRY ) );
	{ // yes
		int flow = _count % MAX_FLOW;
		if ( flow > MAX_FLOW / 2 ) {
			flow = MAX_FLOW - flow;
		}
		if ( !_retry ) {
			flow = 0;
		}
		int select_sx2 = (int)( _select_pos[ 0 ].x + 256 * _select_pos[ 0 ].z );
		int select_sy2 = (int)( _select_pos[ 0 ].y + 128 * _select_pos[ 0 ].z );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( (int)_select_pos[ 0 ].x - flow, (int)_select_pos[ 0 ].y - flow, 0, 0, TEXTURE_X / 2, TEXTURE_Y, select_sx2 + flow, select_sy2 + flow ), GRAPH_YES_NO ) );
	}
	{ // no
		int flow = _count % MAX_FLOW;
		if ( flow > MAX_FLOW / 2 ) {
			flow = MAX_FLOW - flow;
		}
		if ( _retry ) {
			flow = 0;
		}
		int select_sx2 = (int)( _select_pos[ 1 ].x + 256 * _select_pos[ 1 ].z );
		int select_sy2 = (int)( _select_pos[ 1 ].y + 128 * _select_pos[ 1 ].z );
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( (int)_select_pos[ 1 ].x - flow, (int)_select_pos[ 1 ].y - flow, TEXTURE_X / 2, 0, TEXTURE_X / 2, TEXTURE_Y, select_sx2 + flow, select_sy2 + flow ), GRAPH_YES_NO ) );
	}
}

void SceneResult::drawFrame( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );

	//青い太枠
	const int T_SX[ 4 ] = { 0, WIDTH - THICK_FRAME_SIZE, 0, WIDTH - THICK_FRAME_SIZE };
	const int T_SY[ 4 ] = { 0, 0, HEIGHT - THICK_FRAME_SIZE, HEIGHT - THICK_FRAME_SIZE };
	const int T_TX[ 4 ] = { 4, SELECT_SIZE - THICK_FRAME_SIZE - 6, 6, SELECT_SIZE - THICK_FRAME_SIZE - 9 };
	const int T_TY[ 4 ] = { 4, 4, SELECT_SIZE - THICK_FRAME_SIZE - 4, SELECT_SIZE - THICK_FRAME_SIZE - 4 };

	for ( int i = 0; i < 4; i++ ) {
		Drawer::Transform trans( T_SX[ i ], T_SY[ i ], T_TX[ i ], T_TY[ i ], THICK_FRAME_SIZE, THICK_FRAME_SIZE );
		Drawer::Sprite sprite( trans, GRAPH_RESULT_FRAME );
		drawer->setSprite( sprite );
	}

	//白い細枠
	const int SY[ 2 ] = { 11, HEIGHT - 11 };
	const int SY_2[ 2 ] = { 0, HEIGHT };

	const int SX[ 2 ] = { 11, WIDTH - 11 };
	const int SX_2[ 2 ] = { 0, WIDTH - 3 };

	for ( int i = 0; i < 2; i++ ) {
		Drawer::Transform trans( THICK_FRAME_SIZE, SY[ i ], THICK_FRAME_SIZE + 4, 0, SELECT_SIZE - ( THICK_FRAME_SIZE + 11 ) * 2, 11, WIDTH - THICK_FRAME_SIZE, SY_2[ i ] );
		Drawer::Sprite sprite( trans, GRAPH_STAGE_SELECT );
		drawer->setSprite( sprite );
	}
	for ( int i = 0; i < 2; i++ ) {
		Drawer::Transform trans( SX[ i ], THICK_FRAME_SIZE, 0, THICK_FRAME_SIZE + 5, 9, SELECT_SIZE - ( THICK_FRAME_SIZE + 9 ) * 2, SX_2[ i ], HEIGHT - THICK_FRAME_SIZE );
		Drawer::Sprite sprite( trans, GRAPH_STAGE_SELECT );
		drawer->setSprite( sprite );
	}
}

void SceneResult::drawCircle( ) const {
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
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, HEIGHT * 6 / 8 + 10, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	drawer->setSprite( sprite );
}

void SceneResult::selectRetry( ) {
	Sound::getTask( )->stopSE( "alertSE.wav" );
	DevicePtr device = Device::getTask( );
	bool retry = _retry;
	if ( device->getDirX( ) < 0 &&
		 _choice_count == 0 &&
		 _move_count == 0 ) {
		retry = true;
	}
	if ( device->getDirX( ) > 0 &&
		 _choice_count == 0 &&
		 _move_count == 0 ) {
		retry = false;
	}

	if ( retry != _retry ) {
		_retry = retry;
		_move_count++;
	}

	if ( _move_count != 0 ) {
		moveChoice( );
	}

	if ( ( device->getDirY( ) < 0 && device->getRightDirY( ) > 0 ) ||
		 ( device->getDirY( ) > 0 && device->getRightDirY( ) < 0 ) ) {
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
}

void SceneResult::moveChoice( ) {
	ApplicationPtr app = Application::getInstance( );
	int WIDTH = app->getWindowWidth( );
	int HEIGHT = app->getWindowHeight( );
	const int TEXTURE_X = SELECT_TEXTURE_X;
	const int TEXTURE_Y = SELECT_TEXTURE_Y;
	const Vector YES_LEFT  = Vector( WIDTH / 2 - TEXTURE_X / 4 - TEXTURE_X / 2, HEIGHT * 6 / 8 );
	const Vector YES_RIGHT = Vector( WIDTH / 2 - TEXTURE_X / 4, HEIGHT * 6 / 8 );
	const Vector NO_LEFT   = Vector( WIDTH / 2 - TEXTURE_X / 4, HEIGHT * 6 / 8 );
	const Vector NO_RIGHT  = Vector( WIDTH / 2 - TEXTURE_X / 4 + TEXTURE_X * 2 / 3, HEIGHT * 6 / 8 );

	Vector vec[ 2 ];
	if ( _retry ) {
		vec[ 0 ] = YES_RIGHT - YES_LEFT; 
		vec[ 1 ] = NO_RIGHT - NO_LEFT;
		vec[ 0 ] *= ( 1.0 / 10.0 );
		vec[ 1 ] *= ( 1.0 / 10.0 );
	} else {
		vec[ 0 ] = YES_LEFT - YES_RIGHT;
		vec[ 1 ] = NO_LEFT - NO_RIGHT;
		vec[ 0 ] *= ( 1.0 / 10.0 );
		vec[ 1 ] *= ( 1.0 / 10.0 );
	}

	for ( int i = 0; i < 2; i++ ) {
		_select_pos[ i ] += vec[ i ];
		_select_pos[ i ].z = 0;
	}

	double yes_ratio = ( YES_LEFT - _select_pos[ 0 ] ).getLength( ) / ( YES_LEFT - YES_RIGHT ).getLength( );
	_select_pos[ 0 ].z = SELECT_SMALL_SIZE + ( 1.0 - SELECT_SMALL_SIZE ) * yes_ratio;
	double no_ratio = ( NO_RIGHT - _select_pos[ 1 ] ).getLength( ) / ( NO_RIGHT - NO_LEFT ).getLength( );
	_select_pos[ 1 ].z = SELECT_SMALL_SIZE + ( 1.0 - SELECT_SMALL_SIZE ) * no_ratio;

	if ( _move_count == 10 ) {
		_move_count = 0;
	} else {
		_move_count++;
	}
}

void SceneResult::drawCrystalNum( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	int tw = 256;
	int th = 128;

	DrawerPtr drawer = Drawer::getTask( );
	{
		Drawer::Transform trans( WIDTH / 2 - tw * 3 / 4, HEIGHT / 2 - th );
		drawer->setSprite( Drawer::Sprite( trans, GRAPH_RESULT ) );
	}
	{ // 数字
		int max_num = 0;
		if ( _crystal_carry_num / 10 ) {
			max_num = 2;
		} else {
			max_num = 1;
		}

		const int NUM_SIZE = 64;
		for ( int i = 0; i < max_num; i++ ) {
			int sx = ( WIDTH / 2 ) + ( tw / 4 ) + ( i * ( NUM_SIZE * 2 ) );
			int sy = HEIGHT / 2 - NUM_SIZE;
			int sx2 = sx + NUM_SIZE * 2;
			int sy2 = sy + NUM_SIZE * 2;

			int tx = 0;
			int digit = (int)pow( 10, max_num - 1 - i );
			if ( digit > 9 ) {
				tx = ( _crystal_carry_num / digit ) * NUM_SIZE;
			}
			if ( digit < 10 ) {
				tx = ( _crystal_carry_num % 10 ) * NUM_SIZE;
			}

			Drawer::Transform trans( sx, sy, tx, 0, NUM_SIZE, NUM_SIZE, sx2, sy2 );
			drawer->setSprite( Drawer::Sprite( trans, GRAPH_NUMBER ) );
		}
	}
}
