#include "Timer.h"
#include "Drawer.h"
#include "define.h"
#include "Game.h"

static const int FPS = 60;
static const int START_TIME = 10 * FPS;
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
	int x = 720;
	int y = 10;
	const int TW = 32;
	const int TH = 64;
	const int u[ ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int picth = 0;
	int time = _timer / FPS;
	// 強調表示
	bool big = false;
	if ( time < 6 ) {
		big = true;
		x += TW;
	}

	// タイマー表示
	{
		// 小数点以下数字
		Drawer::Sprite sprite( Drawer::Transform( x, y, u[ ( _timer % FPS ) / 6 ] * TW, 0, TW, TH, x + TW + TW * big, y + TH + TH * big ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
	}
	{
		// 小数点
		picth += TW / 2 + TW * big;
		Drawer::Sprite sprite( Drawer::Transform( x - picth, y, 10 * TW, 0, TW / 2, TH, x - picth + TW / 2 + TW * big, y + TH + TH * big ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
		picth += TW + TW * big;
	}
	while ( time >= 0 ) { // 小数点以上数字
		Drawer::Sprite sprite( Drawer::Transform( x - picth, y, u[ time % 10 ] * TW, 0, TW, TH, x - picth + TW + TW * big, y + TH + TH * big ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
		picth += TW + TW * big;
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

void Timer::finalize( ) {
	GamePtr game = Game::getTask( );
	game->setResultTime( _timer * 10 / FPS );
}
