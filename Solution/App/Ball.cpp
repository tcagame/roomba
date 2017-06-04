#include "Ball.h"
#include "AppStage.h"

static const double BALL_RADIUS = WORLD_SCALE * ROOMBA_SCALE;
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
}

void Ball::draw( ) {
	DrawerPtr drawer = Drawer::getTask( );
	//Drawer::ModelMDL model( _pos + adjust, MDL_BALL );
	//drawer->setModelMDL( model );
	Matrix adjust_rot = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), PI / 2 );
	Vector axis = adjust_rot.multiply( _vec.normalize( ) );
	axis = _rot.multiply( axis );
	Matrix pos = Matrix::makeTransformTranslation( _pos );
	Matrix scale = Matrix::makeTransformScaling( BALL_SIZE );
	Matrix rot = Matrix::makeTransformRotation( axis, BALL_MODEL_ROT_SPEED * _vec.getLength( ) );
	Matrix mat;
	_rot = _rot.multiply( rot );
	mat = _rot.multiply( scale );
	mat = mat.multiply( pos );
	//mat.data[3][0] = pos.data[3][0];
	//mat.data[3][1] = pos.data[3][1];
	//mat.data[3][2] = pos.data[3][2];
	//mat.data[0][0] = _rot.data[0][0];
	//mat.data[1][1] = _rot.data[1][1];
	//mat.data[2][2] = _rot.data[2][2];
	//mat.data[0][1] = _rot.data[0][1];
	//mat.data[0][2] = _rot.data[0][2];
	//mat.data[1][0] = _rot.data[1][0];
	//mat.data[1][2] = _rot.data[1][2];
	//mat.data[2][0] = _rot.data[2][0];
	//mat.data[2][1] = _rot.data[2][1];

	Drawer::ModelMV1 model( mat, 0, 0, 0 );
	drawer->setModelMV1( model );
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