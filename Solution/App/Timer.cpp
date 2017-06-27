#include "Timer.h"
#include "define.h"
#include "Game.h"
#include "Application.h"
#include "Sound.h"

const int FPS = 60;
const int START_TIME = 20 * FPS;
const int ADD_TIME = 5 * FPS;
const int RECOVERY_SPEED = 15;

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
	if ( _timer == 5 * FPS ) {
		SoundPtr sound = Sound::getTask( );
	    sound->playSE( "alertSE.wav", true );
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
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, -10, 0, TH, TW, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, -10, 0, 0, ( _timer + BLANK ) * ( TW - BLANK ) / ( START_TIME + BLANK ), TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
}

void Timer::addTime( ) {
	//if ( _timer < 3 * FPS ) {
	//	SoundPtr sound = Sound::getTask( );
	//	sound->playBGM( "bgm_maoudamashii_cyber06.wav" );
	//}
	//_add_time += ADD_TIME;
	//
}

void Timer::reset( ) {
	_timer = START_TIME;
}

bool Timer::isTimeOver( ) const {
	return ( _timer < 0 );
}

void Timer::finalize( ) {
	GamePtr game = Game::getTask( );
	game->setResultTime( _timer * 10 / FPS );
}

int Timer::getTime( ) const {
	return _timer; 
}