#include "Ball.h"
#include "AppStage.h"
#include "Device.h"

static const double BALL_RADIUS = WORLD_SCALE * ROOMBA_SCALE;

Ball::Ball( const Vector& pos ) :
_pos( pos ) {
}


Ball::~Ball( ) {
}

void Ball::update( StagePtr stage ) {
	AppStagePtr stage_ptr = std::dynamic_pointer_cast< AppStage >( stage );
	Vector adjust_vec = stage_ptr->adjustCollisionToWall( _pos, _vec, BALL_RADIUS );
	adjust_vec += stage_ptr->adjustCollisionToCrystal( _pos, _vec, BALL_RADIUS );
	_vec = adjust_vec;
	_vec.z = 0;
	_pos += _vec;
}

void Ball::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMDL model( _pos + Vector( 0, BALL_RADIUS ), MDL_BALL );
	drawer->setModelMDL( model );
}

Vector Ball::getPos( ) const {
	return _pos;
}

Vector Ball::getVec( ) const {
	return _vec;
}

void Ball::setForce( const Vector& force ) {
	_vec = force;
}

void Ball::reset( const Vector& pos ) {
	_vec = Vector( );
	_pos = pos;
}

void Ball::setPos( Vector pos ) {
	_pos = pos;
}