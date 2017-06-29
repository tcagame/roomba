#include "Timer.h"
#include "define.h"
#include "Game.h"
#include "Application.h"
#include "Sound.h"

const int FPS = 60;
const int START_TIME = 30 * FPS;
const int ADD_TIME = 5 * FPS;
const int RECOVERY_SPEED = 8;

Timer::Timer( ) :
_timer( START_TIME ),
_add_time( 0 ) {
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "stageBGM.wav" );
}


Timer::~Timer( ) {
}

void Timer::update( ) {
	_timer--;
	if ( _timer == START_TIME / 4 ) {
		SoundPtr sound = Sound::getTask( );
	    sound->playSE( "alertSE.wav", true );
	}
	if ( _add_time > RECOVERY_SPEED ) {
		_add_time -= RECOVERY_SPEED;
		_timer += RECOVERY_SPEED;
	} else {
		_timer += _add_time;
		_add_time = 0;
	}
}


void Timer::draw( ) const {
	if ( _timer < 0 ) {
		return;
	}
	DrawerPtr drawer = Drawer::getTask( );
	const int TW = 512;
	const int TH = 64;
	const int BLANK = 24;
	int th = 0;
	if ( _timer < START_TIME / 4 && _timer % 6 < 3 ) {
		th = TH * 2;
	}
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, -10, 0, TH, TW, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, -10, 0, th, ( _timer + BLANK ) * ( TW - BLANK ) / ( START_TIME + BLANK ), TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
}

void Timer::addTime( ) {
	_add_time += ADD_TIME;
}


bool Timer::isTimeOver( ) const {
	return ( _timer < 0 );
	Sound::getTask( )->stopSE( "alertSE.wav" );
}

void Timer::finalize( ) {
	GamePtr game = Game::getTask( );
	game->setResultTime( _timer * 10 / FPS );
}

int Timer::getTime( ) const {
	return _timer; 
}