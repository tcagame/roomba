#include "Crystal.h"
#include "Drawer.h"
#include "mathmatics.h"
#include "AppStage.h"
#include "Roomba.h"
#include "Viewer.h"

static const double REFLECTION_POWER = 5.0;
static const double CRYSTAL_RADIUS = 0.5;
static const double MAX_SPEED = 0.9;
static const double DECELERATION = 0.01;

Crystal::Crystal( Vector pos, MDL type ) :
_pos( pos ),
_finished( false ),
_drop_down( false ),
_type( type ) {

}


Crystal::~Crystal( ) {

}

void Crystal::draw( ViewerPtr viewer ) const {
	viewer->drawModelMDL( Drawer::ModelMDL( _pos, _type ) );
}

void Crystal::update( AppStagePtr stage ) {
	_drop_down = false;
	if ( stage->isOnDelivery( _pos ) ) {
		_finished = true;
		return;
	}

	if ( fabs( _pos.z + 2 ) > 0.2 ) {
		int check = 0;
	}

	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
	}
	if ( _vec.getLength( ) > DECELERATION ) {
		_vec -= _vec.normalize( ) * DECELERATION;
	} else {
		_vec = Vector( );
	}

	Vector adjust = stage->adjustCollisionToWall( _pos, _vec, CRYSTAL_RADIUS );
	if ( ( adjust - _vec ).getLength( ) > 0.1 ) {
		_vec = adjust;
		_drop_down = true;
	}

	_pos += _vec;
	if ( _pos.x < STAGE_WIDTH_NUM * WORLD_SCALE - 1 ) {
		_pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	if ( _pos.y < STAGE_HEIGHT_NUM * WORLD_SCALE - 1  ) {
		_pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
}

bool Crystal::isHitting( Vector pos0, Vector pos1 ) {
	//pos0とpos1の間にクリスタルがあるかどうか
	bool hitting = false;
	pos0.z = _pos.z;
	pos1.z = _pos.z;

	double min_x = pos0.x;
	double min_y = pos0.y;
	if ( min_x > pos1.x ) {
		min_x = pos1.x;
	}
	if ( min_y > pos1.y ) {
		min_y = pos1.y;
	}
	Vector pos = _pos;
	while ( min_x > pos.x ) {
		pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( min_y > pos.y ) {
		pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}

	Vector vec0 = pos - pos0;
	Vector vec1 = pos1 - pos0;
	double angle = vec0.angle( vec1 );
	if ( fabs( angle ) > PI / 2 ) {
		return false;
	}
	if ( vec0.getLength( ) > vec1.getLength( ) ) {
		return false;
	}
	double distance = vec0.getLength( ) * fabs( sin( angle ) );
	if ( fabs( distance ) < CRYSTAL_RADIUS ) {
		hitting = true;
		_pos = pos;
	}
	return hitting;
}

Vector Crystal::adjustHitToRoomba( Vector pos, Vector vec, double radius ) {
	int roomba_map_x = (int)( pos.x / WORLD_SCALE ) / STAGE_WIDTH_NUM;
	int roomba_map_y = (int)( pos.y / WORLD_SCALE ) / STAGE_HEIGHT_NUM;
	int crystal_map_x = (int)( _pos.x / WORLD_SCALE ) / STAGE_WIDTH_NUM;
	int crystal_map_y = (int)( _pos.y / WORLD_SCALE ) / STAGE_HEIGHT_NUM;
	Vector tmp_pos = _pos;
	tmp_pos.x += ( roomba_map_x - crystal_map_x ) * STAGE_WIDTH_NUM * WORLD_SCALE;
	tmp_pos.y += ( roomba_map_y - crystal_map_y ) * STAGE_HEIGHT_NUM * WORLD_SCALE;

	Vector tmp_vec = vec;
	Vector future_pos = pos + vec;
	future_pos.z = _pos.z;
	Vector distance = future_pos - tmp_pos;//ボール→クリスタル
	if ( distance.getLength( ) < CRYSTAL_RADIUS + radius ) {
		tmp_vec = distance * -1;
		tmp_vec = tmp_vec.normalize( ) * vec.getLength( );
		_vec = ( tmp_vec - vec ) * -REFLECTION_POWER;
	}
	vec = tmp_vec - vec;
	// クリスタルの反射
	return vec;
}

bool Crystal::isFinished( ) const {
	return _finished;
}

Vector Crystal::getPos( ) const {
	return _pos;
}

void Crystal::setVec( Vector vec ) {
	vec.z = 0;
	_vec = vec;
}

bool Crystal::isDropDown( ) const {
	return _drop_down;
}

void Crystal::Delivery( ) {
	Vector crystal_pos = getPos( );
	crystal_pos.z += 1.5;
}