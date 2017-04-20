#include "Ball.h"
#include "define.h"
#include "Stage.h"
#include "Device.h"

static const double ACCEL = 0.09;
static const double MAX_SPEED = 0.9;
static const double ATTACK_START_SPEED = 0.09;
static const double DECELERATION_SPEED = 0.4;

Ball::Ball( Vector pos ) :
_pos( pos ) {
}


Ball::~Ball( ) {
}

void Ball::update( StagePtr stage ) {
	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
	}
	//if ( stage->isCollisionWall( _pos + _vec + Vector( 1, 1, 0 ) ) ) {
	//	_vec = Vector( );
	//}
	Stage::Collision col = stage->getCollisionWall( _pos, _vec, WORLD_SCALE );
	if ( col.isOverlapped_x ) {
		_pos.x += col.adjust.x;
		_vec.x = 0;
	}
	if ( col.isOverlapped_y ) {
		_pos.y += col.adjust.y;
		_vec.y = 0;
	}

	_pos += _vec;
}

void Ball::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMDL model( _pos, MDL_BALL );
	drawer->setModelMDL( model );
}

Vector Ball::getPos( ) const {
	return _pos;
}

Vector Ball::getVec( ) const {
	return _vec;
}

void Ball::move( Vector camera_dir, Roomba::MOVE_STATE state, BallPtr target ) {

	DevicePtr device = Device::getTask( );
	Vector device_dir;
	Vector other_pos = target->getPos( );

	if ( !_left ) {
		device_dir.x = device->getRightDirX( );
		device_dir.y = device->getRightDirY( );
	}
	if ( _left ) {
		device_dir.x = device->getDirX( );
		device_dir.y = device->getDirY( );
	}

	deceleration( );
	switch ( state ) {
	case Roomba::MOVE_STATE_TRANSLATION:
		moveTranslation( target, device_dir, camera_dir );
		break;
	case Roomba::MOVE_STATE_ROTETION_BOTH:
		moveRotetionBoth( target, device_dir );
		break;
	case Roomba::MOVE_STATE_ROTETION_SIDE:
		moveRotetionSide( target, device_dir );
		break;
	}
}

void Ball::moveTranslation( BallPtr target, Vector device_dir, Vector camera_dir ) {
	Matrix mat = Matrix::makeTransformRotation( camera_dir.cross( Vector( 0, -1 ) ), camera_dir.angle( Vector( 0, -1 ) ) );
	device_dir = mat.multiply( device_dir );
	_vec += device_dir.normalize( ) * ACCEL;
	target->setAccel( target->getVec( ) + ( _vec - target->getVec( ) ) * 0.3 );
}

void Ball::moveRotetionBoth( BallPtr target, Vector device_dir ) {
	if ( fabs( device_dir.x ) > 70 ) {
		device_dir.y = device_dir.x;
	}
	device_dir.x = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector roomba_dir = mat.multiply( target->getPos( ) - _pos );
	mat = Matrix::makeTransformRotation( roomba_dir.cross( Vector( 0, -1 ) ), roomba_dir.angle( Vector( 0, -1 ) ) );
	device_dir = mat.multiply( device_dir );
	if ( !_left ) {
		device_dir *= -1;
	}
	_vec += device_dir.normalize( ) * ACCEL;
}

void Ball::moveRotetionSide( BallPtr target, Vector device_dir ) {
	if ( fabs( device_dir.x ) > 70 ) {
		device_dir.y = device_dir.x;
	}
	device_dir.x = 0;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector roomba_dir = mat.multiply( target->getPos( ) - _pos );
	mat = Matrix::makeTransformRotation( roomba_dir.cross( Vector( 0, -1 ) ), roomba_dir.angle( Vector( 0, -1 ) ) );
	device_dir = mat.multiply( device_dir );
	if ( !_left ) {
		device_dir *= -1;
	}
	_vec += device_dir.normalize( ) * ACCEL;
	if ( device_dir.getLength( ) < 10 ) {
		_vec -= _vec.normalize( ) * ACCEL;
	}
}

void Ball::deceleration( ) {
	//����
	if ( _vec.x > 0 ) {
		_vec.x -= ACCEL / 2;
		if ( _vec.x < 0 ) {
			_vec.x = 0;
		}
	}
	if ( _vec.y > 0 ) {
		_vec.y -= ACCEL / 2;
		if ( _vec.y < 0 ) {
			_vec.y = 0;
		}
	}
	if ( _vec.x < 0 ) {
		_vec.x += ACCEL / 2;
		if ( _vec.x > 0 ) {
			_vec.x = 0;
		}
	}
	if ( _vec.y < 0 ) {
		_vec.y += ACCEL / 2;
		if ( _vec.y > 0 ) {
			_vec.y = 0;
		}
	}
}

void Ball::setAccel( Vector vec ) {
	_vec = vec;
}

void Ball::checkLeft( Vector camera_dir, Vector other_pos ) {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector vec = mat.multiply( other_pos - _pos );
	double dot = camera_dir.dot( vec );
	if ( dot < 0 ) {
		_left = false;
	}
	if ( dot > 0 ) {
		_left = true;
	}
}

bool Ball::isAttacking( ) const {
	return _vec.getLength( ) > ATTACK_START_SPEED;
}

void Ball::reset( Vector pos ) {
	_vec = Vector( );
	_pos = pos;
}