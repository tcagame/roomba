#include "Timer.h"
#include "Drawer.h"
#include "define.h"

static const int FPS = 60;
static const int START_TIME = 3 * FPS;
static const int ADD_TIME = 1;

Timer::Timer( ) :
_timer( START_TIME ) {
}


Timer::~Timer( ) {
}

void Timer::update( ) {
	_timer--;
}

void Timer::draw( ) const {
	if ( _timer < 0 ) {
		return;
	}

	DrawerPtr drawer = Drawer::getTask( );
	int x = 640;
	int y = 10;
	const int TW = 32;
	const int TH = 64;
	const int u[ ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int picth = 0;
	int time = _timer / FPS;
	{
		Drawer::Sprite sprite( Drawer::Transform( x, y, u[ ( _timer % FPS ) / 6 ] * TW, 0, TW, TH, x + TW, y + TH ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
	}
	{
		picth += TW / 2;
		Drawer::Sprite sprite( Drawer::Transform( x - picth, y, 10 * TW, 0, TW / 2, TH, x - picth + TW / 2, y + TH ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
		picth += TW;
	}
	while ( time >= 0 ) {
		Drawer::Sprite sprite( Drawer::Transform( x - picth, y, u[ time % 10 ] * TW, 0, TW, TH, x - picth + TW, y + TH ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
		picth += TW;
		time /= 10;
		
		if ( time == 0 ) {
			break;
		}
	}

}

void Timer::addTime( ) {
	_timer += ADD_TIME * FPS;
}

void Timer::reset( ) {
	_timer = START_TIME;
}

bool Timer::isTimeOver( ) const {
	if ( _timer < 0 ) {
		return true;
	}
	return false;
}
