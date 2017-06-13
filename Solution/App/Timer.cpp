#include "Timer.h"
#include "Drawer.h"
#include "define.h"
#include "Game.h"
#include "Application.h"
#include "Sound.h"

static const int FPS = 60;
static const int START_TIME = 9000 * FPS;
static const int ADD_TIME = 5;

Timer::Timer( ) :
_timer( START_TIME ) {
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "bgm_maoudamashii_cyber06.wav");
}


Timer::~Timer( ) {
}

void Timer::update( ) {
	_timer--;
	
	if ( _timer == 3 * FPS ) {
		SoundPtr sound = Sound::getTask( );
	    sound->playBGM( "meka_ge_keihou.wav" );
	}
}

void Timer::draw( ) const {
	if ( _timer < 0 ) {
		return;
	}
	ApplicationPtr app = Application::getInstance( );
	DrawerPtr drawer = Drawer::getTask( );
	int x = app->getWindowWidth( ) / 2;
	int y = 10;
	const int TW = 32;
	const int TH = 64;
	const int u[ ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int picth = 0;
	int time = _timer / FPS;
	// 強調表示
	bool big = false;
	if ( time < 3 ) {
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
	if ( _timer < 3 * FPS ) {
		SoundPtr sound = Sound::getTask( );
		sound->playBGM( "bgm_maoudamashii_cyber06.wav" );
	}
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
