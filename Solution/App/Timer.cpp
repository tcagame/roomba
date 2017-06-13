#include "Timer.h"
#include "define.h"
#include "Game.h"
#include "Application.h"
#include "Sound.h"

static const int FPS = 60;
static const int START_TIME = 20 * FPS;
static const int ADD_TIME = 5 * FPS;
static const int RECOVERY_SPEED = 10;
static const double MAX_ZOOM = 1.5;
static const double ZOOM_SPEED = 0.02;

Timer::Timer( ) :
_timer( START_TIME ) {
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "bgm_maoudamashii_cyber06.wav" );
}


Timer::~Timer( ) {
}

void Timer::update( ) {
	_timer--;
	updateRecovery( );
	if ( _timer == 3 * FPS ) {
		SoundPtr sound = Sound::getTask( );
	    sound->playBGM( "meka_ge_keihou.wav" );
	}
}

void Timer::updateRecovery( ) {
	std::list< int >::iterator ite = _add_times.begin( );
	_size -= ZOOM_SPEED;
	while ( ite != _add_times.end( ) ) {
		if( (*ite) < RECOVERY_SPEED ) {
			_timer += (*ite);
			(*ite) = 0;
		} else {
			_timer += RECOVERY_SPEED;
			(*ite) -= RECOVERY_SPEED;
		}
		if( (*ite) <= 0 ) {
			ite = _add_times.erase( ite );
		}
		_size += ZOOM_SPEED * 2;
		break;
		ite++;
	}
	if ( _size < 1 ) {
		_size = 1;
	}
	if ( _size > MAX_ZOOM ) {
		_size = MAX_ZOOM;
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
	double size = _size;
	if ( time < 6 ) {
		size *= 2;
		x += TW;
	}

	// タイマー表示
	{
		// 小数点以下数字
		Drawer::Sprite sprite( Drawer::Transform( x, y, u[ ( _timer % FPS ) / 6 ] * TW, 0, TW, TH, x + (int)( TW * size ), y + (int)( TH * size ) ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
	}
	{
		// 小数点
		picth += (int)( TW / 2 * size );
		Drawer::Sprite sprite( Drawer::Transform( x - picth, y, 10 * TW, 0, TW / 2, TH, x - picth + (int)( TW / 2 * size ), y + (int)( TH * size ) ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
		picth += (int)( TW * size );
	}
	while ( time >= 0 ) { // 小数点以上数字
		Drawer::Sprite sprite( Drawer::Transform( x - picth, y, u[ time % 10 ] * TW, 0, TW, TH, x - picth + (int)( TW * size ), y + (int)( TH * size ) ), GRAPH_TIMER_NUM );
		drawer->setSprite( sprite );
		picth += (int)( TW * size );
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
	_add_times.push_back( ADD_TIME );
	
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
