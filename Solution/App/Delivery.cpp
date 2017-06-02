#include "Delivery.h"
#include "Viewer.h"
#include "Crystal.h"
#include "Camera.h"

static const double MOVE_SPEED = 0.4;
static const double START_POS_Z = 20;
static const double MIN_POS_Z = 2;
static const double FOOT = 1;
Delivery::Delivery( Vector target ) :
_target( target ),
_finished( false ),
_have_crystal( false ),
_state( STATE_WAIT ){
	_pos = target;
	_pos.z = START_POS_Z;
}


Delivery::~Delivery( ) {

}

void Delivery::draw( ViewerPtr viewer ) const {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawLine( _pos, _pos - Vector( 0, 0, 10 ) );

	//drawer->setModelMDL( Drawer::ModelMDL( _pos, MDL_DELIVERY ) );
	viewer->drawModelMDL( Drawer::ModelMDL( _pos, MDL_DELIVERY ) );
	if ( _have_crystal ) {
		viewer->drawModelMDL( _crystal );
	}
}

void Delivery::update( CameraPtr camera ) {
	switch ( _state ) {
	case STATE_WAIT:
		updateWait( );
		break;
	case STATE_CATCH:
		updateCatch( );
		break;
	case STATE_LIFT:
		updateLift( camera );
		break;
	case STATE_CARRY:
		updateCarry( );
		break;
	}

}


void Delivery::updateWait( ) {
	Vector vec = _target - _pos;
	if ( vec.getLength( ) > MOVE_SPEED ) {
		_vec = ( _target - _pos ).normalize( ) * MOVE_SPEED;
	} else {
		_vec = vec;
	}
	_pos += _vec;
	
	if ( _pos.z < MIN_POS_Z ) {
		_pos.z = MIN_POS_Z;
	}

	if ( _have_crystal ) {
		_state = STATE_CATCH;
	}
}

void Delivery::updateCatch( ) {
	_vec = _crystal.pos - _pos + Vector( 0, 0, FOOT );
	if ( _vec.getLength( ) > MOVE_SPEED ) {
		_vec = _vec.normalize( ) * MOVE_SPEED;
	}
	_pos += _vec;
	if ( _vec.getLength( ) < 0.01 ) {
		_target = _pos + Vector( 0, 0, 4 );
		_state = STATE_LIFT;
	}
}

void Delivery::updateLift( CameraPtr camera ) {
	//デリバー
	_vec = _target - _pos;
	if ( _vec.getLength( ) > MOVE_SPEED ) {
		_vec = _vec.normalize( ) * MOVE_SPEED;
	}
	if ( _vec.getLength( ) < 0.01 ) {
		_state = STATE_CARRY;
		Vector dir = camera->getDir( );
		dir.z = 0;
		_target = _pos + dir.normalize( ) * 100 * WORLD_SCALE;
	}
	_pos += _vec;

	//クリスタル
	Vector crystal_vec = _pos - _crystal.pos - Vector( 0, 0, FOOT );
	if ( crystal_vec.getLength( ) > MOVE_SPEED * 1.5 ) {
		crystal_vec = crystal_vec.normalize( ) * MOVE_SPEED * 1.5;
	}
	_crystal.pos += crystal_vec;
}

void Delivery::updateCarry( ) {
	//デリバー
	_vec = _target - _pos;
	if ( _vec.getLength( ) > MOVE_SPEED ) {
		_vec = _vec.normalize( ) * MOVE_SPEED;
	}
	if ( _vec.getLength( ) < 0.01 ) {
		_finished = true;
	}
	_pos += _vec;

	//クリスタル
	Vector crystal_vec = _pos - _crystal.pos - Vector( 0, 0, FOOT );
	if ( crystal_vec.getLength( ) > MOVE_SPEED * 1.5 ) {
		crystal_vec = crystal_vec.normalize( ) * MOVE_SPEED * 1.5;
	}
	_crystal.pos += crystal_vec;
}

bool Delivery::isFinished( ) const {
	return _finished;
}

Vector Delivery::getPos( ) const {
	return _pos;
}

void Delivery::setCrystal( Vector pos ) {
	_have_crystal = true;
	_crystal.pos = pos;
	while ( _pos.x - pos.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		_pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( _pos.x - pos.x < -STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		_pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( _pos.y - pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		_pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	while ( _pos.y - pos.y < -STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		_pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	_crystal.type = MDL_CRYSTAL;
}

bool Delivery::isHaveCrystal( ) {
	return _have_crystal;
}

