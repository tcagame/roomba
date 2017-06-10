#include "Crystal.h"
#include "Drawer.h"
#include "mathmatics.h"
#include "AppStage.h"
#include "Roomba.h"
#include "Viewer.h"

static const int MAX_EFFECT_COUNT = 50;
static const double REFLECTION_POWER = 0.3;
static const double CRYSTAL_RADIUS = CRYSTAL_SIZE.x / 3;
static const double MAX_SPEED = 0.7;
static const double DECELERATION = 0.03;
static const double DECELERATION_DROP_DOWN_RATIO = 2;
static const double BOUND_POW = 0.7;

Crystal::Crystal( Vector& pos, MDL type ) :
_pos( pos ),
_start_pos( pos ),
_finished( false ),
_drop_down( true ),
_type( type ),
_effect_count( MAX_EFFECT_COUNT - 1 ) {

}


Crystal::~Crystal( ) {

}

void Crystal::draw( ViewerPtr viewer ) const {
	viewer->drawModelMDL( Drawer::ModelMDL( _pos + Vector( -CRYSTAL_SIZE.x, -CRYSTAL_SIZE.y ), _type ) );
	if ( !_effect_count && _vec == Vector( ) ) {
		Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_CRYSTAL_CIRCLE, _pos + Vector( 0, 0, 1 ), 0.5, EFFECT_ROTATE ) );
	}
}

void Crystal::update( AppStagePtr stage ) {
	if ( stage->isOnDelivery( _pos ) ) {
		_drop_down = true;
		_finished = true;
		return;
	}


	// バウンド
	if ( _pos.z > _start_pos.z ) {
		_vec.z -= GRAVITY;
		if ( _pos.z + _vec.z < _start_pos.z ) {
			if ( fabs( _vec.z ) > GRAVITY * 2 ) {
				_vec.z *= -0.6;
			} else {
				_vec.z = _start_pos.z - _pos.z;
			}
		}
	}

	Vector adjust = stage->adjustCollisionToWall( _pos, _vec, CRYSTAL_RADIUS );
	if ( ( adjust - _vec ).getLength( ) > 0.1 ) {
		_vec = adjust;
		Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_COLLISION_TO_CRYSTAL, _pos, 0.5, EFFECT_ROTATE ) );
		toBound( );
	}


	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
	}
	_pos += _vec;
	if ( _pos.z < _start_pos.z ) {
		_pos.z = _start_pos.z;
	}


	double deceleration = DECELERATION;
	if ( _drop_down ) {
		deceleration *= DECELERATION_DROP_DOWN_RATIO;
	}
	if ( _vec.getLength( ) > DECELERATION ) {
		_vec -= _vec.normalize( ) * deceleration;
	} else {
		_vec = Vector( );
	}

	_effect_count++;
	_effect_count %= MAX_EFFECT_COUNT;
}

bool Crystal::isHitting( Vector pos0, Vector pos1, Vector vec0, Vector vec1 ) {
	if ( _pos.z > _start_pos.z ) {
		return false;
	}
	//pos0とpos1の間にクリスタルがあるかどうか
	Vector crystal_pos = _pos;
	crystal_pos.z = pos0.z;
	if ( vec0.angle( vec1 ) > PI / 2 ) {
		pos1 += vec1;
		vec1 *= -1;
	};

	Vector distance0 = crystal_pos - pos0;
	Vector distance1 = pos1 - pos0;
	double angle = distance0.angle( distance1 );
	if ( fabs( angle ) > PI / 2 ) {
		return false;
	}
	if ( distance0.getLength( ) > distance1.getLength( ) ) {
		return false;
	}
	double distance = distance0.getLength( ) * fabs( sin( angle ) );
	if ( fabs( distance ) < CRYSTAL_RADIUS ) {
		return true;
	}

	//ballの速度が速い時の処理
	Vector pos[ 4 ];
	pos[ 0 ] = pos0;
	pos[ 1 ] = pos0 + vec0;
	pos[ 2 ] = pos1 + vec1;
	pos[ 3 ] = pos1;

	Vector vec_square[ 4 ];
	vec_square[ 0 ] = vec0;
	vec_square[ 1 ] = pos[ 2 ] - pos [ 1 ];
	vec_square[ 2 ] = vec1 * -1;
	vec_square[ 3 ] = pos[ 0 ] - pos[ 3 ];

	Vector vec_to_crystal[ 4 ];
	for ( int i = 0; i < 4; i++ ) {
		vec_to_crystal[ i ] = crystal_pos - pos[ i ];
	}

	Vector cross[ 4 ];
	for ( int i = 0; i < 4; i++ ) {
		cross[ i ] = vec_square[ i ].cross( vec_to_crystal[ i ] );
	}

	if ( ( cross[ 0 ].z < 0 && cross[ 1 ].z < 0 && cross[ 2 ].z < 0 && cross[ 3 ].z < 0 ) ||
		 ( cross[ 0 ].z > 0 && cross[ 1 ].z > 0 && cross[ 2 ].z > 0 && cross[ 3 ].z > 0 ) ) {
		_pos = crystal_pos;
		return true;
	}
	return false;
}

Vector Crystal::adjustHitToCircle( Vector pos, Vector vec, double radius ) {
	if ( !_drop_down && radius > 0 ) {
		return Vector( );
	}
	if ( radius < 0 ) {
		radius = CRYSTAL_RADIUS;
	}
	pos.z = _pos.z;
	Vector tmp_vec = vec;
	Vector future_pos = pos + vec;
	Vector distance = future_pos - _pos;//ボール→クリスタル
	if ( distance.getLength( ) < CRYSTAL_RADIUS + radius ) {
		tmp_vec = ( pos - _pos ).normalize( ) * vec.getLength( );
		// クリスタルの反射
		_vec = ( tmp_vec - vec ).normalize( ) * -REFLECTION_POWER;
		toBound( );
	}
	vec = tmp_vec - vec;
	return vec;
}

bool Crystal::isFinished( ) const {
	return _finished;
}

const Vector& Crystal::getPos( ) const {
	return _pos;
}

const Vector& Crystal::getVec( ) const {
	return _vec;
}

void Crystal::setVec( Vector& vec ) {
	_vec = vec;
}

bool Crystal::isDropDown( ) const {
	return _drop_down;
}

void Crystal::setDropDown( bool drop_down ) {
	_drop_down = drop_down;
}

void Crystal::shiftPos( Vector& base_pos ) {
	while ( _pos.x - base_pos.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		_pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( _pos.x - base_pos.x < -STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		_pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( _pos.y - base_pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		_pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	while ( _pos.y - base_pos.y < -STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		_pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
}

void Crystal::toBound( ) {
	if ( !( _pos.z > _start_pos.z ) ) {
		_vec.z = BOUND_POW;
		_drop_down = true;
	}
}