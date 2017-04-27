#include "Ball.h"
#include "define.h"
#include "Stage.h"
#include "Device.h"

static const double MAX_SPEED = 1.1;

Ball::Ball( const Vector& pos ) :
_pos( pos ) {
}


Ball::~Ball( ) {
}

void Ball::update( StagePtr stage ) {
	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
	}
	Vector adjust_vec = stage->getCollisionWall( _pos, _vec, BALL_RADIUS );
	_vec = adjust_vec;
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

void Ball::deceleration( const double accel ) {
	double length = _vec.getLength( );
	length -= accel / 2;
	if ( length < 0 ) {
		length = 0;
	}
	_vec = _vec.normalize( ) * length;
}

void Ball::addForce( const Vector& force ) {
	_vec += force;
}

void Ball::reset( const Vector& pos ) {
	_vec = Vector( );
	_pos = pos;
}