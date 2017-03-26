#include "Timer.h"
#include "Drawer.h"
#include "define.h"

static const int MAX_TIME = 10 * 60;

Timer::Timer( ) :
_timer( MAX_TIME ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
}


Timer::~Timer( ) {
}

void Timer::update( ) {
	_timer--;
	if ( _timer < 0 ) {
		_timer = 0;
	}
}

void Timer::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	int x = 640;
	int y = 10;
	const int TW = 32;
	const int TH = 64;
	const int u[ ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int picth = 0;
	for ( int i = _timer / 60; i > 0; ) {
		Drawer::Sprite sprite = Drawer::Sprite( Drawer::Transform( x - picth, y, u[ i % 10 ] * TW, 0, TW, TH, x - picth + TW, y + TH ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
		i /= 10;
		picth += 50;
	}

}

bool Timer::isTimeOver( ) const {
	bool result = false;
	if ( _timer == 0 ) {
		result = true;
	}
	return result;
}
