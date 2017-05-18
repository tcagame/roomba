#include "Crystal.h"
#include "Drawer.h"
#include "mathmatics.h"
#include "AppStage.h"
#include "Roomba.h"

static const double CRYSTAL_RADIUS = 0.5;
static const double MAX_SPEED = 0.8;
static const double DECELERATION = 0.2;

Crystal::Crystal( Vector pos, MDL type ) :
_pos( pos ),
_finished( false ),
_drop_down( false ),
_type( type ) {

}


Crystal::~Crystal( ) {

}

void Crystal::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMDL model( _pos, _type );
	drawer->setModelMDL( model );
}

void Crystal::update( AppStagePtr stage ) {
	_drop_down = false;
	if ( stage->isOnStation( _pos, (int)( _type - MDL_CRYSTAL_0 ) + 1 ) ) {
		_finished = true;
	}

	Vector adjust = stage->adjustCollisionToWall( _pos, _vec, CRYSTAL_RADIUS );
	if ( ( adjust - _vec ).getLength( ) > 0.1 ) {
		_vec = adjust;
		_drop_down = true;
	}

	if ( _vec.getLength( ) > DECELERATION ) {
		_vec -= _vec.normalize( ) * DECELERATION;
	} else {
		_vec = Vector( );
	}
	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
	}
	_pos += _vec;
}

bool Crystal::isHitting( Vector pos0, Vector pos1 ) {
	//pos0とpos1の間にクリスタルがあるかどうか
	bool hitting = false;
	Vector pos = _pos;
	pos.z = pos0.z;
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
	}
	return hitting;
}

Vector Crystal::adjustHitToRoomba( Vector pos, Vector vec, double radius ) {
	//pos0とpos1の間にクリスタルがあるかどうか
	bool hitting = false;
	Vector tmp_vec = vec;
	Vector future_pos = pos + vec;
	future_pos.z = _pos.z;
	Vector distance = future_pos - _pos;//ボール→クリスタル
	if ( distance.getLength( ) < CRYSTAL_RADIUS + radius ) {
		Vector tmp_vec = distance * -1;
		vec = tmp_vec.normalize( ) * vec.getLength( );
	}
	return tmp_vec - vec;
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

MDL Crystal::getType( ) const {
	return _type;
}
