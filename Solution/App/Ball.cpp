#include "Ball.h"
#include "AppStage.h"
#include "Sound.h"

static const double EFFECT_COLLISION_CRYSTAL_SIZE = 0.5;
static const double EFFECT_COLLISION_WALL_SIZE = 0.5;
static const double BALL_MODEL_ROT_SPEED = PI / 3;

Ball::Ball( const Vector& pos ) :
_pos( pos ),
_collision_count( 0 ),
_reflection( false ) {
}


Ball::~Ball( ) {
}

void Ball::update( const Vector& vec, StagePtr stage, bool rot, bool check_collision ) {
	_vec = vec;

	SoundPtr sound = Sound::getTask( );
	AppStagePtr stage_ptr = std::dynamic_pointer_cast< AppStage >( stage );
	Vector adjust_vec_to_wall = _vec;
	Vector adjust_vec_to_crystal = Vector( );
	if ( check_collision ) {
		adjust_vec_to_wall = stage_ptr->adjustCollisionToWall( _pos, _vec, BALL_RADIUS );
		adjust_vec_to_crystal = stage_ptr->adjustCollisionToCrystal( _pos, _vec, BALL_RADIUS );
	}
	Vector adjust_vec = adjust_vec_to_wall + adjust_vec_to_crystal;

	if ( ( _vec - adjust_vec ).getLength( ) > 0.001 ) {
		_reflection = true;
		if ( adjust_vec_to_crystal != Vector( ) ) {
			Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_COLLISION_TO_CRYSTAL, _pos, EFFECT_COLLISION_CRYSTAL_SIZE, EFFECT_ROTATE ) );
			sound->playSE( "collision.wav" );
		} else {
			Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_COLLISION_TO_WALL, _pos, EFFECT_COLLISION_WALL_SIZE, EFFECT_ROTATE ) );
			sound->playSE( "collision.wav" );
			_collision_count++;
		}
	}
	_vec = adjust_vec;
	_pos += _vec;
	if ( rot ) {
		Matrix adjust_rot = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), PI / 2 );
		Vector axis = adjust_rot.multiply( _vec.normalize( ) );
		axis = _rot.multiply( axis );
		Matrix rot = Matrix::makeTransformRotation( axis, BALL_MODEL_ROT_SPEED * _vec.getLength( ) );
		_rot = _rot.multiply( rot );
	}
}

void Ball::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMV1 model( getMat( _pos ), MV1_BALL, 0, 0 );
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

void Ball::setPos( Vector& pos ) {
	_pos = pos;
}

bool Ball::isReflection( ) const {
	return _reflection;
}

void Ball::setReflection( bool ref ) {
	_reflection = ref;
}

int Ball::getColCount( ) const {
	return _collision_count;
}