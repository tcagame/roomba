#include "SceneResult.h"
#include "Drawer.h"
#include "Device.h"
#include "define.h"
#include "Application.h"
#include "Binary.h"

static const int RESULT_WIDTH  = 512;
static const int RESULT_HEIGHT = 256;
static const int RESULT_CENTER_X = 1450 / 2;
static const int RESULT_CENTER_Y = 80;
static const int THIS_TIME_Y = 200;
static const int BEST_TIME_Y = 400;
static const std::string DIRECTORY = "../Resource/Savedata/";
static const std::string FILENAME = DIRECTORY + "best_time.dat";

SceneResult::SceneResult( int time ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_RESULT, "Result/result.png" );
	drawer->loadGraph( GRAPH_TIMER_NUM, "Result/time.png" );
	_this_time = time;
	_best_time = 0;
	loadBestTime( );
}


SceneResult::~SceneResult( ) {
}

Scene::NEXT SceneResult::update( ) {
	draw( );
	DevicePtr device = Device::getTask( );
	if ( device->getPush( ) && BUTTON_D ) {
		return NEXT_TITLE;
	}
	return NEXT_CONTINUE;
}

void SceneResult::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );

	Drawer::Sprite sprite( Drawer::Transform( RESULT_CENTER_X - RESULT_WIDTH / 2, RESULT_CENTER_Y - RESULT_HEIGHT / 2, 0, 0, 512, 256 ), GRAPH_RESULT );
	drawer->setSprite( sprite );
	drawThisTime( );
	drawBestTime( );
}


int SceneResult::drawTime( int x, int y, int time ) const {
	DrawerPtr drawer = Drawer::getTask( );
	const int WIDTH = 32;
	const int HEIGHT = 64;
	const int SIZE = 2;
	int x2 = x + WIDTH * SIZE;
	int y2 = y + HEIGHT * SIZE;
	// 小数点以下数字
	Drawer::Transform trans( x, y, ( time % 10 ) * WIDTH, 0, WIDTH, HEIGHT, x2, y2 );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_TIMER_NUM ) );
	time /= 10;
	// 小数点
	x -= WIDTH * SIZE / 2;
	x2 = x + WIDTH * SIZE / 2;
	trans =  Drawer::Transform( x, y, 10 * WIDTH, 0, WIDTH / 2, HEIGHT, x2, y2 );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_TIMER_NUM ) );
	
	// 小数点以上数字
	x -= WIDTH * SIZE;
	x2 = x + WIDTH * SIZE;
	while ( time >= 0 ) {
		trans =  Drawer::Transform( x, y, ( time % 10 ) * WIDTH, 0, WIDTH, HEIGHT, x2, y2 );
		drawer->setSprite( Drawer::Sprite( trans, GRAPH_TIMER_NUM ) );
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
	int wnd_width = app->getWindowWidth( );
	int x = wnd_width / 3 * 2;
	int y = THIS_TIME_Y;
	x = drawTime( x, y, _this_time );
	x -= 400;//文字サイズ
	y -= 60;
	Drawer::Transform trans =  Drawer::Transform( x, y, 512, 0, 512, 256 );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_RESULT ) );
}

void SceneResult::drawBestTime( ) const {
	ApplicationPtr app = Application::getInstance( );
	int wnd_width = app->getWindowWidth( );
	int x = wnd_width / 3 * 2;
	int y = BEST_TIME_Y;
	x = drawTime( x, y, _best_time );
	x -= 400;//文字サイズ
	y -= 60;
	Drawer::Transform trans =  Drawer::Transform( x, y, 512, 256, 512, 256 );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_RESULT ) );
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