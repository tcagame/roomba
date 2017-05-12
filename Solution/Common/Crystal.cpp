#include "Crystal.h"
#include "Drawer.h"
#include "define.h"
#include "mathmatics.h"
#include "Stage.h"

static const double CRYSTAL_RADIUS = 0.5;
static const double MAX_SPEED = 0.8;

Crystal::Crystal( Vector pos ) :
_pos( pos ),
_finished( false ) {

}


Crystal::~Crystal( ) {

}

void Crystal::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMDL model( _pos, MDL::MDL_CRYSTAL );
	drawer->setModelMDL( model );
}

void Crystal::update( StagePtr stage ) {
	if ( _vec.getLength( ) > MAX_SPEED ) {
		_vec = _vec.normalize( ) * MAX_SPEED;
	}
	_vec = stage->getCollisionWall( _pos, _vec, CRYSTAL_RADIUS	);
	_pos += _vec;
}

bool Crystal::isHitting( Vector pos0, Vector pos1 ) {
	//pos0‚Æpos1‚ÌŠÔ‚ÉƒNƒŠƒXƒ^ƒ‹‚ª‚ ‚é‚©‚Ç‚¤‚©
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

void Crystal::damage( ) {
	_finished = true;
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