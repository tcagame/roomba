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
const std::string DIRECTORY = "../Resource/Savedata/";
const std::string FILENAME = DIRECTORY + "best_time.dat";
const int BRANK = 500;
static const int THICK_FRAME_SIZE = 57;
static const int SELECT_SIZE  = 512;
static const int CIRCLE_ANIME_FLAME = 1;
static const int MAX_CHOICE_COUNT = 25 * CIRCLE_ANIME_FLAME;
static const int DRAW_TIME = 100;

SceneResult::SceneResult( int time, int col_num ) :
_select( 1 ),
_choice_count( 0 ),
_count( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_CONTROLLER_NEUTRAL, "controller/neutral.png" );
	drawer->loadGraph( GRAPH_CONTROLLER_ROTATION, "controller/rotation.png" );
	drawer->loadGraph( GRAPH_OK, "UI/ok.png" );
	drawer->loadGraph( GRAPH_CIRCLE, "scene/circle.png" );
	drawer->loadGraph( GRAPH_STAGE_SELECT, "select/Stage Select.png" );
	drawer->loadGraph( GRAPH_GAME_OVER, "UI/game_over.png" );
	drawer->loadGraph( GRAPH_GAME_CLEAR, "UI/StageClear.png" );
	drawer->loadGraph( GRAPH_RESULT_FRAME, "UI/game_over_frame.png" );


	_this_time = time;
	_best_time = 0;
	loadBestTime( );
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
			return NEXT_TITLE;
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
	return NEXT_CONTINUE;
}

void SceneResult::draw( ) const {
	//drawThisTime( );
	//drawBestTime( );
	//drawBg( );
	drawFadeBg( );
	drawResult( );
	drawGameClear( );
	drawFrame( );
	drawCircle( );
	if ( getFadeInCount( ) < MAX_FADE_COUNT ) {
		drawFadeIn( );
	} else {
		drawFadeOut( );
	}
	
}

void SceneResult::drawBg( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( 0, 0, 0, 0, 1920, 1080, WIDTH, HEIGHT ), GRAPH_BG );
	drawer->setSprite( sprite );
}

void SceneResult::drawResult( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int RESULT_WIDTH = 169;
	const int RESULT_HEIGHT = 37;

	Drawer::Transform trans( WIDTH / 2 - RESULT_WIDTH / 2, HEIGHT / 2 - RESULT_HEIGHT * 8, 427, 50, RESULT_WIDTH, RESULT_HEIGHT );
	Drawer::Sprite sprite( trans, GRAPH_GAME_OVER );
	drawer->setSprite( sprite );
}

void SceneResult::drawGameClear( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	const int GAMECLEAR_WIDTH = 655;
	const int GAMECLEAR_HEIGHT = 80;

	Drawer::Transform trans( WIDTH / 2 - GAMECLEAR_WIDTH  / 2, HEIGHT / 2 - GAMECLEAR_HEIGHT / 2, 192, 114, GAMECLEAR_WIDTH, GAMECLEAR_HEIGHT );

	Drawer::Sprite sprite( trans, GRAPH_GAME_CLEAR );
	drawer->setSprite( sprite );
}


int SceneResult::drawTime( int x, int y, int time ) const {
	DrawerPtr drawer = Drawer::getTask( );
	const int WIDTH = 64;
	const int HEIGHT = 64;
	const int SIZE = 2;
	int x2 = x + WIDTH * SIZE;
	int y2 = y + HEIGHT * SIZE;
	// 小数点以下数字
	Drawer::Transform trans( x, y, ( time % 10 ) * WIDTH, 0, WIDTH, HEIGHT, x2, y2 );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_NUMBER ) );
	time /= 10;
	// 小数点
	x -= WIDTH * SIZE / 2;
	x2 = x + WIDTH * SIZE / 2;
	trans =  Drawer::Transform( x, y, 10 * WIDTH, 0, WIDTH / 2, HEIGHT, x2, y2 );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_NUMBER ) );
	
	// 小数点以上数字
	x -= WIDTH * SIZE;
	x2 = x + WIDTH * SIZE;
	while ( time >= 0 ) {
		trans =  Drawer::Transform( x, y, ( time % 10 ) * WIDTH, 0, WIDTH, HEIGHT, x2, y2 );
		drawer->setSprite( Drawer::Sprite( trans, GRAPH_NUMBER ) );
		x -= WIDTH * SIZE;
		x2 = x + WIDTH * SIZE;
		time /= 10;
		
		if ( time == 0 ) {
			break;
		}
	}
	return x;
}

void SceneResult::drawThisTime( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	int x = WIDTH / 3 * 2;
	int y = HEIGHT * 2 / 5;
	x = drawTime( x, y, _this_time );
	x -= 400;//文字サイズ
	y -= 60;
	Drawer::Transform trans =  Drawer::Transform( x, y, 512, 0, 512, 256 );
	DrawerPtr drawer = Drawer::getTask( );
	//drawer->setSprite( Drawer::Sprite( trans, GRAPH_RESULT ) );
}

void SceneResult::drawBestTime( ) const {
	ApplicationPtr app = Application::getInstance();
	const int WIDTH = app->getWindowWidth();
	const int HEIGHT = app->getWindowHeight();
	int x = WIDTH / 3 * 2;
	int y = HEIGHT * 3 / 5;
	x = drawTime( x, y, _best_time );
	x -= 400;//文字サイズ
	y -= 60;
	Drawer::Transform trans =  Drawer::Transform( x, y, 512, 256, 512, 256 );
	DrawerPtr drawer = Drawer::getTask( );
	//drawer->setSprite( Drawer::Sprite( trans, GRAPH_RESULT ) );
}

void SceneResult::loadBestTime( ) {
	//load
	ApplicationPtr app = Application::getInstance( );
	BinaryPtr binary( new Binary );
	if ( !app->loadBinary( FILENAME, binary ) ) {
		saveBestTime( );
		return;
	}
	_best_time = *(int*)binary->getPtr( );
	binary->~Binary( );

	if ( _best_time < _this_time ) {
		saveBestTime( );
	}
}

void SceneResult::saveBestTime( ) {
	ApplicationPtr app = Application::getInstance( );
	BinaryPtr binary( new Binary );
	binary->ensure( sizeof( _this_time ) );
	binary->write( (void*)&_this_time, sizeof( _this_time ) );
	app->saveBinary( FILENAME, binary );
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
	if ( idx > 25 || getFadeOutCount( ) != MAX_FADE_COUNT  ) {
		idx = 25;
	}
	int tx = idx % 5;
	int ty = idx / 5;

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( WIDTH / 2 - CIRCLE_SIZE / 2, HEIGHT / 2 - 74 + CIRCLE_SIZE * 3 / 5, tx * CIRCLE_SIZE, ty * CIRCLE_SIZE, CIRCLE_SIZE, CIRCLE_SIZE ), GRAPH_CIRCLE );
	drawer->setSprite( sprite );
}
