#include "SceneSelect.h"
#include "Device.h"
#include "define.h"
#include "Drawer.h"
#include "Game.h"
#include "Application.h"
#include "Sound.h"

static const int SELECT_SIZE  = 512;
static const int SELECT_CENTER_X = 430 / 2;
static const int SELECT_CENTER_Y = 42 / 2;
static const int NUM_SIZE = 256;
static const int NUM_CENTER = NUM_SIZE / 2;
static const int PITCH = 150; 
static const int MOVE_SPEED = 1;
static const int MAX_MOVE_COUNT = 10;
static const int CIRCLE_ANIME_FLAME = 1;
static const int MAX_CHOICE_COUNT = 25 * CIRCLE_ANIME_FLAME;
static const int THICK_FRAME_SIZE = 57;
static const int TRIANGLE_CENTER_X = 312 / 2;
static const int DRAW_TIME = 100;

SceneSelect::SceneSelect( bool open_stage ) :
_select( 0 ),
_count( 0 ),
_choice_count( 0 ),
_move_count( 0 ),
_ispush( false ),
_open_stage( open_stage ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_CONTROLLER_NEUTRAL, "controller/neutral.png" );
	drawer->loadGraph( GRAPH_CONTROLLER_ROTATION, "controller/rotation.png" );
	drawer->loadGraph( GRAPH_OK, "UI/select_ok.png" );
	drawer->loadGraph( GRAPH_BG, "title/title_bg.png" );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle_blue1.png" );
	drawer->loadGraph( GRAPH_STAGE_SELECT, "select/Stage Select.png" );
	drawer->loadGraph( GRAPH_SELECT, "select/Select.png" );
	drawer->loadGraph( GRAPH_BEGINNER, "select/beginner_1.png" );
	if ( _open_stage ) {
		drawer->loadGraph( GRAPH_NORMAL, "select/normal.png" );
		drawer->loadGraph( GRAPH_NUM_3, "select/3.png" );
	} else {
		drawer->loadGraph( GRAPH_NORMAL, "select/close.png" );
		drawer->loadGraph( GRAPH_NUM_3, "select/close.png" );
	}
	freazeSelect( );
	resetCount( );
}


SceneSelect::~SceneSelect( ) {
}

Scene::NEXT SceneSelect::update( ) {
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
			GamePtr game = Game::getTask( );
			game->setStage( _select );
			return NEXT_STAGE;
		}
	}

	// サークルカウント
	if ( ( right_stick.y > 0 && left_stick.y < 0 ) ||
		 ( right_stick.y < 0 && left_stick.y > 0 ) ) {
		_choice_count++;
		if ( _choice_count == 1 ) {
			sound->playSE( "circleSE.wav" );
		}
	} else {
		_choice_count = 0;
		sound->stopSE( "circleSE.wav" );
	}

	//　ステージ番号選択
	if ( _move_count == MAX_MOVE_COUNT - 3 ) {
		if ( _rot_right ) {
			_select++;
		} else {
			_select--;
		}
	}

	if ( _move_count == 0 && _choice_count == 0 && getFadeOutCount( ) == MAX_FADE_COUNT ) {
		freazeSelect( );
		if ( left_stick.x > 0 && !_ispush ) {
			sound->playSE( "selectSE.wav" );
			_move_count++;
			_rot_right = true;
			_ispush = true;
		}
		if ( left_stick.x < 0 && !_ispush ) {
			sound->playSE( "selectSE.wav" );
			_move_count++;
			_rot_right = false;
			_ispush = true;
		}
	}
	if ( left_stick.x == 0 && _ispush ) {
		_ispush = false;
	}

	if ( _move_count != 0 ) {
		moveSelect( );
		_move_count++;
		_move_count %= MAX_MOVE_COUNT;
	} else if ( !_open_stage && _select != 0 ) { // ステージ選択できないようにする
		_move_count++;
		_ispush = true;
		sound->playSE( "selectSE.wav" );
	}

	if ( _select < 0 ) { 
		_select = _select % 3 + 3;
	}
	if ( _select >= 3 ) {
		_select = _select % 3;
	}
	return NEXT_CONTINUE;
}

void SceneSelect::draw( ) {
	//drawFadeBg( );
	drawBg( );
	drawRogo( );
	drawOk( );
	drawSelect( );
	drawTriangle( );
	drawFrame( );
	
	drawCircle( );
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		drawFadeIn( );
	} else {
		drawFadeOut( );
	}

}

void SceneSelect::drawBg( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( 0, 0, 0, 0, 1920, 1080, WIDTH, HEIGHT ), GRAPH_BG );
	drawer->setSprite( sprite );
}

void SceneSelect::drawRogo( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - SELECT_CENTER_X, HEIGHT / 5, 46, 86, 430, 50 ), GRAPH_STAGE_SELECT );
	drawer->setSprite( sprite );
	
}

void SceneSelect::drawSelect( ) {
	DrawerPtr drawer = Drawer::getTask( );
	int select = _select;

	if ( select == 0 ) {
		select = 3;
	}


	GRAPH graph = (GRAPH)( GRAPH_BEGINNER + select );
	
	//真ん中
	if ( graph == (GRAPH)( GRAPH_NUM_3 + 1 ) ) {
		graph = GRAPH_BEGINNER;
	}
	if ( graph == (GRAPH)( GRAPH_BEGINNER - 1 ) ) {
		graph = GRAPH_NUM_3;
	}
	Drawer::Sprite sprite_1( Drawer::Transform( (int)_pos[ 0 ].x, (int)_pos[ 0 ].y, 0, 0, NUM_SIZE, NUM_SIZE  ), graph );
	drawer->setSprite( sprite_1 );

	//左
	graph = (GRAPH)( graph + 1 );
	if ( graph == (GRAPH)( GRAPH_NUM_3 + 1 ) ) {
		graph = GRAPH_BEGINNER;
	}
	if ( graph == (GRAPH)( GRAPH_BEGINNER - 1 ) ) {
		graph = GRAPH_NUM_3;
	}
	Drawer::Sprite sprite_2( Drawer::Transform( (int)_pos[ 1 ].x, (int)_pos[ 1 ].y, 0, 0, NUM_SIZE, NUM_SIZE, (int)_pos[ 1 ].x + NUM_SIZE * 2 / 5, (int)_pos[ 1 ].y + NUM_SIZE * 2 / 5 ), graph );
	drawer->setSprite( sprite_2 );

	//右
	graph = (GRAPH)( graph + 1 );
	if ( graph == (GRAPH)( GRAPH_NUM_3 + 1 ) ) {
		graph = GRAPH_BEGINNER;
	}
	if ( graph == (GRAPH)( GRAPH_BEGINNER - 1 ) ) {
		graph = GRAPH_NUM_3;
	}
	Drawer::Sprite sprite_3( Drawer::Transform( (int)_pos[ 2 ].x, (int)_pos[ 2 ].y, 0, 0, NUM_SIZE, NUM_SIZE, (int)_pos[ 2 ].x + NUM_SIZE * 2 / 5, (int)_pos[ 2 ].y + NUM_SIZE * 2 / 5 ), graph );
	drawer->setSprite( sprite_3 );
	
}

void SceneSelect::moveSelect( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	const Vector target1( WIDTH / 2 - NUM_CENTER,  HEIGHT / 2 - NUM_CENTER );
	const Vector target2( target1.x - NUM_CENTER * 3, target1.y );
	const Vector target3( target1.x + NUM_CENTER * 4, target1.y );
	Vector vec1;
	Vector vec2;
	Vector vec3;
	if ( _rot_right ) {
		vec1 = ( target3 - target1 ) * ( 1.0 / MAX_MOVE_COUNT );
		vec2 = ( target1 - target2 ) * ( 1.0 / MAX_MOVE_COUNT );
		vec3 = ( target2 - target3 ) * ( 1.0 / MAX_MOVE_COUNT );
	} else {
		vec1 = ( target2 - target1 ) * ( 1.0 / MAX_MOVE_COUNT );
		vec2 = ( target3 - target2 ) * ( 1.0 / MAX_MOVE_COUNT );
		vec3 = ( target1 - target3 ) * ( 1.0 / MAX_MOVE_COUNT );
	}

	_pos[ 0 ] += vec1;
	_pos[ 1 ] += vec2;
	_pos[ 2 ] += vec3;
}

void SceneSelect::freazeSelect( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	_pos[ 0 ] = Vector( WIDTH / 2 - NUM_CENTER, HEIGHT / 2 - NUM_CENTER );
	_pos[ 1 ] = Vector( _pos[ 0 ].x - NUM_CENTER * 3, _pos[ 0 ].y );
	_pos[ 2 ] = Vector( _pos[ 0 ].x + NUM_CENTER * 4, _pos[ 0 ].y  );
}

void SceneSelect::drawFrame( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );

	//黄色い太枠
	const int T_SX[ 4 ] = { 0, WIDTH - THICK_FRAME_SIZE, 0, WIDTH - THICK_FRAME_SIZE };
	const int T_SY[ 4 ] = { 0, 0, HEIGHT - THICK_FRAME_SIZE, HEIGHT - THICK_FRAME_SIZE };
	const int T_TX[ 4 ] = { 4, SELECT_SIZE - THICK_FRAME_SIZE - 6, 6, SELECT_SIZE - THICK_FRAME_SIZE - 9 };
	const int T_TY[ 4 ] = { 4, 4, SELECT_SIZE - THICK_FRAME_SIZE - 4, SELECT_SIZE - THICK_FRAME_SIZE - 4 };

	for ( int i = 0; i < 4; i++ ) {
		Drawer::Transform trans( T_SX[ i ], T_SY[ i ], T_TX[ i ], T_TY[ i ], THICK_FRAME_SIZE, THICK_FRAME_SIZE );
		Drawer::Sprite sprite( trans, GRAPH_STAGE_SELECT );
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

void SceneSelect::drawTriangle( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	{
		Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - NUM_CENTER - 70, HEIGHT / 2, 0, 0, 64, 64 ), GRAPH_SELECT );
		drawer->setSprite( sprite );
	}
	{
		Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 + NUM_CENTER + 6, HEIGHT / 2, 64, 0, 64, 64 ), GRAPH_SELECT );
		drawer->setSprite( sprite );
	}
}

void SceneSelect::drawCircle( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int CIRCLE_SIZE = 100;
	int idx = _choice_count / CIRCLE_ANIME_FLAME;
	if ( idx > 25 || getFadeOutCount( ) != MAX_FADE_COUNT  ) {
		idx = 25;
	}
	int tx = idx % 5;
	int ty = idx / 5;
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, HEIGHT * 6 / 8 - 16, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	//Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, HEIGHT / 2 - 74 + CIRCLE_SIZE * 3 / 5, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	drawer->setSprite( sprite );
}

void SceneSelect::drawOk( ) {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	int tx = 168;
	int ty = 366;
	int tw = 167;
	int th = 74;

	DrawerPtr drawer = Drawer::getTask( );
	//Drawer::Transform trans2( WIDTH / 2 - tw / 2, HEIGHT * 5 / 6, tx, ty,  tw,  th, WIDTH / 2 + tw / 2,  HEIGHT * 5 / 6 + th );
	Drawer::Transform trans2( WIDTH / 2 - tw / 2, HEIGHT * 6 / 8, tx, ty,  tw,  th, WIDTH / 2 + tw / 2,  HEIGHT * 6 / 8 + th );
	Drawer::Sprite sprite( trans2, GRAPH_OK );
	drawer->setSprite( sprite );
}
