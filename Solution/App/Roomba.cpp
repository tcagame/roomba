#include "Roomba.h"
#include "define.h"
#include "Ball.h"
#include "AppCamera.h"
#include "AppStage.h"
#include "Crystal.h"
#include "Device.h"
#include <assert.h>

static const int ON_NUTRAL_TIME = 3;
static const double SPEED = 0.18;
static const double MAX_SPEED = 0.7;
static const double CENTRIPETAL_POWER = 0.03;
static const double CENTRIPETAL_MIN = 7;
static const double MAX_SCALE = 25;
static const double MIN_SCALE = 8;


static const Vector START_POS[ 2 ] {
	Vector( 15, 15 ) * WORLD_SCALE,
	Vector( 20, 20 ) * WORLD_SCALE
};

Roomba::Roomba( ) :
_state( MOVE_STATE_NEUTRAL ) {
	_balls[ BALL_LEFT ] = BallPtr( new Ball( START_POS[ 0 ] ) );
	_balls[ BALL_RIGHT ] = BallPtr( new Ball( START_POS[ 1 ] ) );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage, CameraPtr camera ) {
	updateState( camera );
	move( camera );
	for ( int i = 0; i < 2; i++ ) {
		if ( _force[ i ].getLength2( ) > MAX_SPEED * MAX_SPEED ) {
			_force[ i ] = _force[ i ].normalize( ) * MAX_SPEED;
		}
		_balls[ i ]->setForce( _force[ i ] );
		_balls[ i ]->update( stage );
	}

	//レーザー
	holdCrystal( stage );
}

void Roomba::move( CameraPtr camera ) {	
	Vector camera_dir = camera->getDir( );
	camera_dir.z = 0;
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	switch ( _state ) {
	case MOVE_STATE_NEUTRAL:
		// caution
		/*
		for ( int i = 0; i < 2; i++ ) {
			_balls[ i ]->deceleration( SPEED );
		}
		*/
		break;
	case MOVE_STATE_TRANSLATION:
		moveTranslation( camera_dir, right_stick, left_stick );
		break;
	case MOVE_STATE_ROTATION_RIGHT:
		moveRotation( -1 );
		break;
	case MOVE_STATE_ROTATION_LEFT:
		moveRotation( 1 );
		break;
	}
}

void Roomba::updateState( CameraPtr camera ) {
	DevicePtr device = Device::getTask( );
	Vector right_stick( device->getRightDirX( ), device->getRightDirY( ) );
	Vector left_stick( device->getDirX( ), device->getDirY( ) );
	
	MOVE_STATE state = MOVE_STATE_NEUTRAL;
	if ( right_stick.y > 0  && left_stick.y < 0 ) {
		state = MOVE_STATE_ROTATION_RIGHT;
	}
	if ( right_stick.y < 0  && left_stick.y > 0 ) {
		state = MOVE_STATE_ROTATION_LEFT;
	}
	if ( ( right_stick.x < 0 && left_stick.x < 0 ) ||
		 ( right_stick.x > 0 && left_stick.x > 0 ) ||
		 ( right_stick.y < 0 && left_stick.y < 0 ) ||
		 ( right_stick.y > 0 && left_stick.y > 0 ) ) {
		state = MOVE_STATE_TRANSLATION;
	}
	if ( state == MOVE_STATE_NEUTRAL ) {
		_neutral++;
	} else {
		_neutral = 0;
	}
	if ( state != _state ) {
		if ( state == MOVE_STATE_NEUTRAL &&
			 _neutral < ON_NUTRAL_TIME ) {
			state = _state;
		}
		_state = state;
		checkLeftRight( camera );
	}
}

void Roomba::holdCrystal( StagePtr stage ) {
	AppStagePtr app_stage = std::dynamic_pointer_cast< AppStage >( stage );
	if ( !_crystal ) {
		_crystal =  app_stage->getHittingCrystal( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
	}
	if ( _crystal ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 300, 20, "もってるよー" );
		_crystal->setVec( getCentralPos( ) - _crystal->getPos( ) );

		if ( _crystal->isDropDown( ) ) {
			_crystal = CrystalPtr( );
		}
	}
}

void Roomba::moveTranslation( const Vector& camera_dir, const Vector& right, const Vector& left ) {
	Matrix mat = Matrix::makeTransformRotation( camera_dir.cross( Vector( 0, -1 ) ), camera_dir.angle( Vector( 0, -1 ) ) );

	Vector vec_left  = mat.multiply( left  ).normalize( ) * SPEED;
	Vector vec_right = mat.multiply( right ).normalize( ) * SPEED;
	Vector scale_left = Vector( );
	Vector scale_right = Vector( );

	if ( fabs( vec_left.x ) > fabs( vec_left.y ) ) {
		scale_left.y = vec_left.y;
		vec_left.y = 0;
	} else {
		scale_left.x = vec_left.x;
		vec_left.x = 0;
	}
	if ( fabs( vec_right.x ) > fabs( vec_right.y ) ) {
		scale_right.y = vec_right.y;
		vec_right.y = 0;
	} else {
		scale_right.x = vec_right.x;
		vec_right.x = 0;
	}
	
	moveScale( scale_left, scale_right );
	addForceLeft( vec_left );
	addForceRight( vec_right );
}

void Roomba::moveScale( Vector scale_left, Vector scale_right ) {
	Vector vec_left = scale_left;
	Vector vec_right = scale_right;
	Vector scale = _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( );

	if ( ( vec_left + vec_right + scale ).getLength( ) > MAX_SCALE ) { 
		Vector left_dir = _balls[ BALL_RIGHT ]->getPos( ) - _balls[ BALL_LEFT ]->getPos( );
		Vector right_dir = _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( );
		vec_left = left_dir.normalize( );
		vec_right = right_dir.normalize( );
	}
	if ( ( vec_left + vec_right + scale ).getLength( ) < MIN_SCALE ) { 
		Vector left_dir = _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( );
		Vector right_dir = _balls[ BALL_RIGHT ]->getPos( ) - _balls[ BALL_LEFT ]->getPos( );
		vec_left = left_dir.normalize( ) * ( ( vec_left + vec_right + scale ).getLength( ) * 0.5 );
		vec_right = right_dir.normalize( ) * ( ( vec_left + vec_right + scale ).getLength( ) * 0.5 );
	}

	addForceLeft( vec_left );
	addForceRight( vec_right );
}

void Roomba::moveRotation( int rotation_dir ) {
	for ( int i = 0;  i < 2; i++ ) {
		Vector radius = _balls[ i ]->getPos( ) - getCentralPos( );
		Matrix mat_rot = Matrix::makeTransformRotation( Vector( 0, 0, rotation_dir ), SPEED / radius.getLength( ) );
		radius = mat_rot.multiply( radius );
		Vector vec = ( ( radius + getCentralPos( ) ) - _balls[ i ]->getPos( ) );
		if ( i == 0 ) {
			addForceLeft( vec );		
		} else {
			addForceRight( vec );
		}
	}
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < 2; i++ ) {
		_balls[ i ]->draw( );
	}
	drawer->drawLine( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
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
	_balls[ BALL_LEFT ]->reset( START_POS[ 0 ] );
	_balls[ BALL_RIGHT ]->reset( START_POS[ 1 ] );
	_crystal = CrystalPtr( );
}

void Roomba::checkLeftRight( CameraPtr camera ) {	
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector vec = mat.multiply( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) );
	double dot = camera->getDir( ).dot( vec );
	if ( dot > 0 ) {
		BallPtr tmp = _balls[ BALL_LEFT ];
		_balls[ BALL_LEFT ] = _balls[ BALL_RIGHT ];
		_balls[ BALL_RIGHT ] = tmp;
	}
}

void Roomba::addForceLeft( const Vector& force ) {
	_force[ BALL_LEFT ] += force;
}

void Roomba::addForceRight( const Vector& force ) {
	_force[ BALL_RIGHT ] += force;
}