#include "Timer.h"
#include "define.h"
#include "Game.h"
#include "Application.h"
#include "Sound.h"

static const int FPS = 60;
static const int START_TIME = 20 * FPS;
static const int ADD_TIME = 5 * FPS;
static const int RECOVERY_SPEED = 15;

Timer::Timer( ) :
_timer( START_TIME ) {
	SoundPtr sound = Sound::getTask( );
	sound->playBGM( "bgm_maoudamashii_cyber06.wav");
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
		break;
		ite++;
	}
}

void Timer::draw( ) const {
	if ( _timer < 0 ) {
		return;
	}
	DrawerPtr drawer = Drawer::getTask( );
	const int TW = 400;
	const int TH = 50;
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, 10, 0, TH, TW, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
	drawer->setSprite( Drawer::Sprite( Drawer::Transform( 10, 10, 0, 0, _timer * TW / START_TIME, TH ), GRAPH_LINK_GAUGE, Drawer::BLEND_ALPHA, 0.9 ) );
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
