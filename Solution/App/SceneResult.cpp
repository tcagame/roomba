#include "SceneResult.h"
#include "Drawer.h"
#include "Device.h"
#include "define.h"
#include "Application.h"

static const int RESULT_WIDTH  = 512;
static const int RESULT_HEIGHT = 256;
static const int RESULT_CENTER_X = 1450 / 2;
static const int RESULT_CENTER_Y = 80;
static const int TIME_Y = 200;

SceneResult::SceneResult( int time ) :
_time( time ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_RESULT, "Result/result.png" );
	drawer->loadGraph( GRAPH_TIMER_NUM, "Result/time.png" );
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

	Drawer::Sprite sprite( Drawer::Transform( RESULT_CENTER_X - RESULT_WIDTH / 2, RESULT_CENTER_Y - RESULT_HEIGHT / 2, 0, 0, 512, 512 ), GRAPH_RESULT );
	drawer->setSprite( sprite );
	
	drawTimer( );
}


void SceneResult::drawTimer( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	ApplicationPtr app = Application::getInstance( );
	int wnd_width = app->getWindowWidth( );
	const int WIDTH = 32;
	const int HEIGHT = 64;
	const int SIZE = 2;
	int x = wnd_width / 3 * 2;
	int y = TIME_Y;
	int x2 = x + WIDTH * SIZE;
	int y2 = y + HEIGHT * SIZE;
	int time = _time;
	// ¬”“_ˆÈ‰º”Žš
	Drawer::Transform trans( x, y, ( time % 10 ) * WIDTH, 0, WIDTH, HEIGHT, x2, y2 );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_TIMER_NUM ) );
	time /= 10;
	// ¬”“_
	x -= WIDTH * SIZE / 2;
	x2 = x + WIDTH * SIZE / 2;
	trans =  Drawer::Transform( x, y, 10 * WIDTH, 0, WIDTH / 2, HEIGHT, x2, y2 );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_TIMER_NUM ) );
	
	// ¬”“_ˆÈã”Žš
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
	x -= 400;
	y -= 60;
	trans =  Drawer::Transform( x, y, 512, 0, 512, 512 );
	drawer->setSprite( Drawer::Sprite( trans, GRAPH_RESULT ) );
}