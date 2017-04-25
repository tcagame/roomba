#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Ball.h"
#include "Camera.h"
#include "Crystal.h"
#include "Timer.h"
#include "Device.h"

static const double ACCEL = 0.18;
static const double MAX_SPEED = 0.9;
static const double ATTACK_START_SPEED = 0.09;

static const Vector START_POS[ 2 ] {
	Vector( 10, 6 ) * WORLD_SCALE + Vector( 0, 0, 0 ),
	Vector( 10, 6 ) * WORLD_SCALE + Vector( WORLD_SCALE * 4, 0, 0 )
};
static const double CENTRIPETAL_POWER = 0.01;
static const double CENTRIPETAL_MIN = 7;

Roomba::Roomba( ) :
_state( MOVE_STATE_TRANSLATION ) {
	_balls[ 0 ] = BallPtr( new Ball( START_POS[ 0 ] ) );
	_balls[ 1 ] = BallPtr( new Ball( START_POS[ 1 ] ) );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera, TimerPtr timer ) {
	checkLeftRight( camera );
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
	/*


		// ボールの加速度を計算

	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->move( camera_dir, _state, _balls[ i % 2 == 0 ] );
		// ボールの加速度に求心力を加算
		Vector pos0 = _balls[ i ]->getPos( ) + _balls[ i ]->getVec( );
		Vector pos1 = _balls[ ( i == 0 ) ]->getPos( );
		Vector distance = pos1 - pos0;
		Vector vec = distance - distance.normalize( ) * CENTRIPETAL_MIN;
		if ( distance.getLength( ) < CENTRIPETAL_MIN ) {
			Vector target_pos = pos1 - distance.normalize( ) * CENTRIPETAL_MIN;
			_balls[ i ]->setAccel( target_pos - _balls[ i ]->getPos( ) );
			continue;
		}
		vec *= CENTRIPETAL_POWER;
		if ( _balls[ i ]->getVec( ).getLength( ) < 0.1 ) {
			vec *= ( 0.2 / CENTRIPETAL_POWER );
		}
		_balls[ i ]->setAccel( _balls[ i ]->getVec( ) + vec );
	}
	*/
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	switch ( _state ) {
	case MOVE_STATE_TRANSLATION:
		moveTranslation( camera_dir, right_stick, left_stick );
		break;
	}
}

void Roomba::updateState( CameraPtr camera ) {
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	
	MOVE_STATE state = MOVE_STATE_TRANSLATION;
	if ( right_stick.x > 0 ||
		 right_stick.x <  0 ||
		 left_stick.x > 0 ||
		 left_stick.x < 0 || 
		 right_stick.y > 0 ||
		 right_stick.y < 0 ||
		 left_stick.y > 0 ||
		 left_stick.y < 0 ) {
		state = MOVE_STATE_ROTATION_SIDE;
	}
	if ( ( right_stick.x > 0  && left_stick.x < 0 ) ||
		 ( right_stick.x < 0  && left_stick.x > 0 ) ||
		 ( right_stick.y > 0  && left_stick.y < 0 ) ||
		 ( right_stick.y < 0  && left_stick.y > 0 ) ) {
		state = MOVE_STATE_ROTATION_BOTH;
	}
	if ( ( right_stick.x < 0 && left_stick.x < 0 ) ||
		 ( right_stick.x > 0 && left_stick.x > 0 ) ||
		 ( right_stick.y < 0 && left_stick.y < 0 ) ||
		 ( right_stick.y > 0 && left_stick.y > 0 ) ) {
		state = MOVE_STATE_TRANSLATION;
	}
	if ( state != _state ) {
		_state = state;
	/*	_balls[ 0 ]->checkLeft( camera->getDir( ), _balls[ 1 ]->getPos( ) );
		_balls[ 1 ]->checkLeft( camera->getDir( ), _balls[ 0 ]->getPos( ) );
	*/}
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

void Roomba::moveTranslation( const Vector& inAxis, Vector right, Vector left ) {
	Matrix mat = Matrix::makeTransformRotation( inAxis.cross( Vector( 0, -1 ) ), inAxis.angle( Vector( 0, -1 ) ) );
	for ( int i = 0; i < 2; i++ ) {
		Vector dir = left;
		if ( i == 1 ) {
			dir = right;
		}
		dir = mat.multiply( dir );
		Vector vec = dir.normalize( ) * ACCEL;
		_balls[ i ]->setAccel( _balls[ i ]->getVec( ) + vec );
	}
}

void Roomba::moveRotetionBoth( ) {

}

void Roomba::moveRotetionSide( ) {

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

void Roomba::checkLeftRight( CameraPtr camera ) {	
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector vec = mat.multiply( _balls[ 0 ]->getPos( ) - _balls[ 1 ]->getPos( ) );
	double dot = camera->getDir( ).dot( vec );
	if ( dot < 0 ) {
		BallPtr tmp = _balls[ 0 ];
		_balls[ 0 ] = _balls[ 1 ];
		_balls[ 1 ] = tmp;
	}
}