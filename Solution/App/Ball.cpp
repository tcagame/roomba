#include "Ball.h"
#include "AppStage.h"

static const double BALL_RADIUS = WORLD_SCALE * ROOMBA_SCALE;
static const Vector adjust( -ROOMBA_SCALE, 1 - BALL_RADIUS );

Ball::Ball( const Vector& pos ) :
_pos( pos ),
_reflection( false ) {
}


Ball::~Ball( ) {
}

void Ball::update( const Vector& vec, StagePtr stage ) {
	_vec = vec;

	AppStagePtr stage_ptr = std::dynamic_pointer_cast< AppStage >( stage );
	Vector adjust_vec = stage_ptr->adjustCollisionToWall( _pos, _vec, BALL_RADIUS );
	adjust_vec += stage_ptr->adjustCollisionToCrystal( _pos, _vec, BALL_RADIUS );
	if ( _vec != adjust_vec ) {
		_reflection = true;
	}
	_vec = adjust_vec;
	if ( _vec == Vector( ) ) {
		_reflection = false;
	}
	_vec.z = 0;
	_pos += _vec;
}

void Ball::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMDL model( _pos + adjust, MDL_BALL );
	drawer->setModelMDL( model );
}

Vector Ball::getPos( ) const {
	return _pos;
}

Vector Ball::getVec( ) const {
	return _vec;
}

void Ball::reset( const Vector& pos ) {
	_vec = Vector( );
	_pos = pos;
}

void Ball::setPos( Vector pos ) {
	_pos = pos;
}

bool Ball::isReflection( ) const {
	return _reflection;
}