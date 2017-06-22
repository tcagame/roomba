#include "Delivery.h"
#include "Viewer.h"
#include "Crystal.h"
#include "Camera.h"
#include "Sound.h"
#include "Shadow.h"

static const double MOVE_SPEED = 0.1;
static const double START_POS_Z = 20;
static const double MIN_POS_Z = 2;
static const double FOOT = 0.3;
static const double EFFECT_POINT_SIZE = 0.5;
static const double EFFECT_CATCH_SIZE = 0.5;
static const int MAX_EFFECT_COUNT = 30;

Delivery::Delivery( Vector target ) :
_target( target ),
_finished( false ),
_have_crystal( false ),
_state( STATE_WAIT ),
_effect_count( MAX_EFFECT_COUNT - 1 ) {
	_pos = target;
	_pos.z = START_POS_Z;
	_animation = AnimationPtr( new Animation( Animation::ANIM::ANIM_DELIVERY_STAND ) );
}


Delivery::~Delivery( ) {

}

void Delivery::draw( ViewerPtr viewer ) const {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawLine( _pos, _pos - Vector( 0, 0, 4 ) );
	Matrix scale = Matrix::makeTransformScaling( DELIVERY_SIZE );
	Matrix rot = Matrix::makeTransformRotation( Vector( 1, 0, 0 ), PI / 2 );
	Stage::MV1_INFO model = _animation->getModel( );
	if ( _have_crystal ) {
		Matrix trans = Matrix::makeTransformTranslation( _pos );
		drawer->setModelMV1( Drawer::ModelMV1( scale.multiply( rot ).multiply( trans ), model.type, 0, model.time ) );
		drawer->setModelMDL( _crystal );
	} else {
		model.pos = _pos;
		viewer->drawModelMV1( model, scale.multiply( rot ) );
	}
	
	if ( _state == STATE_WAIT && !_effect_count ) {
		drawer->setEffect( Drawer::Effect( EFFECT_DELIVERY_POINT, Vector( _pos.x, _pos.y, 0 ), EFFECT_POINT_SIZE, EFFECT_ROTATE ) );
	}
}

void Delivery::update( CameraPtr camera, ShadowPtr shadow ) {
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
	_animation->update( );
	shadow->set( _animation->getPos( ) );
}


void Delivery::updateWait( ) {
	_effect_count++;
	_effect_count %= MAX_EFFECT_COUNT;
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
		if ( _animation->getAnim( ) == Animation::ANIM::ANIM_DELIVERY_STAND ) {
			_animation->changeAnim( Animation::ANIM::ANIM_DELIVERY_CATCH );
		}
		if ( _animation->getAnim( ) == Animation::ANIM::ANIM_DELIVERY_CARRY ) {
			_target = _pos + Vector( 0, 0, 2 );
			_state = STATE_LIFT;
			Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_CATCH_CRYSTAL, _pos, EFFECT_CATCH_SIZE, EFFECT_ROTATE ) );
		}
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
	if ( _have_crystal ) {
		return;
	}
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