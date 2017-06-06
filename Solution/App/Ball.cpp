#include "Ball.h"
#include "AppStage.h"

static const Vector BALL_SIZE( 0.1, 0.1, 0.1 );
static const double BALL_MODEL_ROT_SPEED = PI / 3;
static const Vector adjust( -ROOMBA_SCALE, 1 - BALL_RADIUS );

Ball::Ball( const Vector& pos ) :
_pos( pos ),
_reflection( false ) {
}


Ball::~Ball( ) {
}

void Ball::update( const Vector& vec, StagePtr stage ) {
	_before_vec = _vec;
	_vec = vec;

	AppStagePtr stage_ptr = std::dynamic_pointer_cast< AppStage >( stage );
	Vector adjust_vec = stage_ptr->adjustCollisionToWall( _pos, _vec, BALL_RADIUS );
	adjust_vec += stage_ptr->adjustCollisionToCrystal( _pos, _vec, BALL_RADIUS );
	if ( _vec != adjust_vec ) {
		_reflection = true;
	}
	_vec = adjust_vec;
	if ( _vec.normalize( ) != _before_vec.normalize( ) ) {
		_reflection = false;
	}
	_vec.z = 0;
	_pos += _vec;
	Matrix adjust_rot = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), PI / 2 );
	Vector axis = adjust_rot.multiply( _vec.normalize( ) );
	axis = _rot.multiply( axis );
	Matrix rot = Matrix::makeTransformRotation( axis, BALL_MODEL_ROT_SPEED * _vec.getLength( ) );
	_rot = _rot.multiply( rot );
}

void Ball::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMV1 model( getMat( _pos ), 0, 0, 0 );
	drawer->setModelMV1( model );
}

Matrix Ball::getMat( const Vector& pos ) const {
	Matrix mat_pos = Matrix::makeTransformTranslation( pos );
	Matrix scale = Matrix::makeTransformScaling( BALL_SIZE );
	Matrix mat;
	mat = _rot.multiply( scale );
	mat = mat.multiply( mat_pos );
	return mat;
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