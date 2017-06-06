#include "Crystal.h"
#include "Drawer.h"
#include "mathmatics.h"
#include "AppStage.h"
#include "Roomba.h"
#include "Viewer.h"

static const double REFLECTION_POWER = 5.0;
static const double CRYSTAL_RADIUS = crystal_size.x / 3;
static const double MAX_SPEED = 0.9;
static const double DECELERATION = 0.01;
static const double BOUND_POW = 0.1;

Crystal::Crystal( Vector pos, MDL type ) :
_pos( pos ),
_start_pos( pos ),
_finished( false ),
_drop_down( false ),
_type( type ) {

}


Crystal::~Crystal( ) {

}

void Crystal::draw( ViewerPtr viewer ) const {
	viewer->drawModelMDL( Drawer::ModelMDL( _pos + Vector( -WORLD_SCALE / 8, -WORLD_SCALE / 8 ), _type ) );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawLine( _pos, _pos + Vector( 0, 0, 4 ) );
}

void Crystal::update( AppStagePtr stage ) {
	if ( stage->isOnDelivery( _pos ) ) {
		_drop_down = true;
		_finished = true;
		return;
	}

	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
	}

	// バウンド
	if ( _pos.z > _start_pos.z ) {
		_vec.z -= DECELERATION;
		if ( _pos.z + _vec.z < _start_pos.z ) {
			_vec.z = _start_pos.z - _pos.z;
		}
	}

	// バウンド
	if ( _drop_down &&
		 _pos.z == _start_pos.z &&
		 _vec.x != 0 && _vec.y != 0 ) {
		_vec.z = BOUND_POW;
	}

	Vector adjust = stage->adjustCollisionToWall( _pos, _vec, CRYSTAL_RADIUS );
	if ( ( adjust - _vec ).getLength( ) > 0.1 ) {
		_vec = adjust;
		_drop_down = true;
		// バウンド
		_vec.z = BOUND_POW;
	}

	_pos += _vec;
	if ( _vec.getLength( ) > DECELERATION ) {
		_vec -= _vec.normalize( ) * DECELERATION;
	} else {
		_vec = Vector( );
		_drop_down = false;
	}
}

bool Crystal::isHitting( Vector pos0, Vector pos1, Vector vec0, Vector vec1 ) {
	//pos0とpos1の間にクリスタルがあるかどうか
	Vector crystal_pos = _pos;
	while ( crystal_pos.x - pos0.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		crystal_pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( crystal_pos.x - pos0.x < -STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		crystal_pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( crystal_pos.y - pos0.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		crystal_pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	while ( crystal_pos.y - pos0.y < -STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		crystal_pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}

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
		_pos = crystal_pos;
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

Vector Crystal::adjustHitToRoomba( Vector pos, Vector vec, double radius ) {
	pos.z = _pos.z;
	if ( !_drop_down ) {
		return Vector( );
	}
	Vector tmp_vec = vec;
	Vector future_pos = pos + vec;
	Vector distance = future_pos - _pos;//ボール→クリスタル
	if ( distance.getLength( ) < CRYSTAL_RADIUS + radius ) {
		tmp_vec = ( pos - _pos ).normalize( ) * vec.getLength( );
		// クリスタルの反射
		_vec = ( tmp_vec - vec ) * -REFLECTION_POWER;
	}
	vec = tmp_vec - vec;
	return vec;
}

bool Crystal::isFinished( ) const {
	return _finished;
}

Vector Crystal::getPos( ) const {
	return _pos;
}

void Crystal::setVec( Vector vec ) {
	_vec = vec;
}

bool Crystal::isDropDown( ) const {
	return _drop_down;
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