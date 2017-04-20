#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Ball.h"
#include "Camera.h"
#include "Crystal.h"
#include "Timer.h"
#include "Device.h"

static const Vector START_POS[ 2 ] {
	Vector( 2, 6 ) * WORLD_SCALE + Vector( 0, 0, 0 ),
	Vector( 2, 6 ) * WORLD_SCALE + Vector( WORLD_SCALE * 2, 0, 0 )
};
static const double CENTRIPETAL_POWER = 0.02;
static const double CENTRIPETAL_MIN = 3.5;

Roomba::Roomba( ) :
_state( MOVE_STATE_TRANSLATION ) {
	_balls[ 0 ] = BallPtr( new Ball( START_POS[ 0 ] ) );
	_balls[ 1 ] = BallPtr( new Ball( START_POS[ 1 ] ) );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera, TimerPtr timer ) {
	updateState( camera );
	move( stage, camera );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->update( stage );
	}

	//攻撃
	attack( stage, timer );
}

void Roomba::move( StagePtr stage, CameraPtr camera ) {
	Vector camera_dir = camera->getDir( );
	camera_dir.z = 0;



		// ボールの加速度を計算

	for ( int i = 0; i < 2; i++ ) {

		_balls[ i ]->move( camera_dir, _state, _balls[ i % 2 == 0 ] );
		// ボールの加速度に求心力を加算
		Vector vec = getCentralPos( ) - _balls[ i ]->getPos( );
		if ( vec.getLength( ) < CENTRIPETAL_MIN ) {
			continue;
		}
		vec -= vec.normalize( ) * CENTRIPETAL_MIN;
		vec *= CENTRIPETAL_POWER;
		_balls[ i ]->setAccel( _balls[ i ]->getVec( ) + vec );
	}
}

void Roomba::updateState( CameraPtr camera ) {
	DevicePtr device = Device::getTask( );
	int dir_lx = device->getDirX( );
	int dir_ly = device->getDirY( );
	int dir_rx = device->getRightDirX( );
	int dir_ry = device->getRightDirY( );

	MOVE_STATE state = MOVE_STATE_TRANSLATION;
	if ( dir_ry > 0 ||
		 dir_ry < 0 ||
		 dir_ly > 0 ||
		 dir_ly < 0 || 
		 dir_rx > 0 ||
		 dir_rx < 0 ||
		 dir_lx > 0 ||
		 dir_lx < 0 ) {
		state = MOVE_STATE_ROTETION_SIDE;
	}
	if ( ( dir_ry > 0  && dir_ly < 0 ) ||
		 ( dir_ry < 0  && dir_ly > 0 ) ||
		 ( dir_rx > 0  && dir_lx < 0 ) ||
		 ( dir_rx < 0  && dir_lx > 0 ) ) {
		state = MOVE_STATE_ROTETION_BOTH;
	}
	if ( ( dir_rx < 0 && dir_lx < 0 ) ||
		 ( dir_rx > 0 && dir_lx > 0 ) ||
		 ( dir_ry < 0 && dir_ly < 0 ) ||
		 ( dir_ry > 0 && dir_ly > 0 ) ) {
		state = MOVE_STATE_TRANSLATION;
	}
	if ( state != _state ) {
		_state = state;
		_balls[ 0 ]->checkLeft( camera->getDir( ), _balls[ 1 ]->getPos( ) );
		_balls[ 1 ]->checkLeft( camera->getDir( ), _balls[ 0 ]->getPos( ) );
	}
}

void Roomba::attack( StagePtr stage, TimerPtr timer ) {
	bool attacking = ( _balls[ 0 ]->isAttacking( ) || _balls[ 1 ]->isAttacking( ) );

	if ( !attacking ) {
		return;
	}
	CrystalPtr crystal =  stage->getHittingCrystal( _balls[ 0 ]->getPos( ), _balls[ 1 ]->getPos( ) );
	if ( crystal ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 0, 0, "あたってるよー" );
		crystal->damage( );
		timer->addTime( );
	}
}

void Roomba::draw( ) const {
	bool attacking = ( _balls[ 0 ]->isAttacking( ) || _balls[ 1 ]->isAttacking( ) );

	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->draw( );
	}

	if ( attacking ) {
		drawer->drawLine( _balls[ 0 ]->getPos( ), _balls[ 1 ]->getPos( ) );
	}
}

Vector Roomba::getCentralPos( ) const {
	Vector pos[ 2 ];
	for ( int i = 0; i < 2; i++ ) {
		pos[ i ] = _balls[ i ]->getPos( );
	}
	Vector central_pos = ( pos[ 0 ] + pos[ 1 ] ) * 0.5;

	return central_pos;
}

void Roomba::reset( ) {
	_balls[ 0 ]->reset( START_POS[ 0 ] );
	_balls[ 1 ]->reset( START_POS[ 1 ]  );
}