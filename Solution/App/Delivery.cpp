#include "Delivery.h"
#include "Viewer.h"
#include "Crystal.h"
#include "Camera.h"
#include "Sound.h"
#include "Shadow.h"

static const double MOVE_SPEED = 0.2;
static const double START_POS_Z = 20;
static const double FOOT = 0.3;
static const double LIFT_UP_Z = 6;
static const double EFFECT_POINT_SIZE = 0.5;
static const double EFFECT_CATCH_SIZE = 0.5;
static const int MAX_EFFECT_COUNT = 50;

Delivery::Delivery( Vector target ) :
_target( target ),
_finished( false ),
_have_crystal( false ),
_state( STATE_WAIT ),
_effect_count( MAX_EFFECT_COUNT - 1 ) {
	_animation = AnimationPtr( new Animation( Animation::ANIM::ANIM_DELIVERY_STAND ) );
	Vector pos = target;
	pos.z = START_POS_Z;
	_animation->setPos( pos );
}


Delivery::~Delivery( ) {

}

void Delivery::draw( ViewerPtr viewer ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Vector pos = _animation->getPos( );
	const double SHADOW_DELIVERY_SCALE = ( DELIVERY_SIZE.x * SUN_POS ) / ( SUN_POS - pos.z );
	if ( _have_crystal ) {
		const double SHADOW_CRYSTAL_SCALE = ( CRYSTAL_SIZE.x * SUN_POS ) / ( SUN_POS - _crystal.pos.z );
		_animation->draw( );
		drawer->setModelMDL( _crystal );
		viewer->setShadow( pos, SHADOW_DELIVERY_SCALE, false );
		viewer->setShadow( _crystal.pos, SHADOW_CRYSTAL_SCALE, false );
	} else {
		_animation->draw( viewer );
		viewer->setShadow( pos, SHADOW_DELIVERY_SCALE );
	}
	
	if ( _state == STATE_WAIT && !_effect_count ) {
		pos.z = 0;
		drawer->setEffect( Drawer::Effect( EFFECT_DELIVERY_POINT, pos, EFFECT_POINT_SIZE, EFFECT_ROTATE ) );
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
	
	move( );
	_animation->update( );
}

void Delivery::move( ) {
	if ( _vec.getLength( ) > MOVE_SPEED ) {
		_vec = _vec.normalize( ) * MOVE_SPEED;
	}
	Vector pos = _animation->getPos( );
	pos += _vec;
	_animation->setPos( pos );
}

void Delivery::updateWait( ) {
	_effect_count++;
	_effect_count %= MAX_EFFECT_COUNT;
	Vector pos = _animation->getPos( );
	_vec = _target - pos;
}

void Delivery::updateCatch( ) {
	Vector pos = _animation->getPos( );
	_vec = _target - pos;
	if ( _vec.getLength( ) < 0.01 ) {
		if ( _animation->getAnim( ) == Animation::ANIM::ANIM_DELIVERY_STAND ) {
			_animation->changeAnim( Animation::ANIM::ANIM_DELIVERY_CATCH );
		}
		if ( _animation->getAnim( ) == Animation::ANIM::ANIM_DELIVERY_CARRY ) {
			_target = pos + Vector( 0, 0, LIFT_UP_Z );
			_state = STATE_LIFT;
			SoundPtr sound = Sound::getTask( );
			sound->playSE( "magic04.wav" );
			Drawer::getTask( )->setEffect( Drawer::Effect( EFFECT_CATCH_CRYSTAL, pos, EFFECT_CATCH_SIZE, EFFECT_ROTATE ) );
		}
	}
}

void Delivery::updateLift( CameraPtr camera ) {
	//デリバー
	Vector pos = _animation->getPos( );
	_vec = _target - pos;
	if ( _vec.getLength( ) > MOVE_SPEED ) {
		_vec = _vec.normalize( ) * MOVE_SPEED;
	}
	if ( _vec.getLength( ) < 0.01 ) {
		_state = STATE_CARRY;
		Vector dir = camera->getDir( );
		dir.z = 0;
		_target = pos + dir.normalize( ) * 100 * WORLD_SCALE;
	}

	//クリスタル
	Vector crystal_vec = pos - _crystal.pos - Vector( 0, 0, FOOT );
	if ( crystal_vec.getLength( ) > MOVE_SPEED * 1.5 ) {
		crystal_vec = crystal_vec.normalize( ) * MOVE_SPEED * 1.5;
	}
	_crystal.pos += crystal_vec;
}

void Delivery::updateCarry( ) {
	//デリバー
	Vector pos = _animation->getPos( );
	_vec = _target - pos;
	if ( _vec.getLength( ) > MOVE_SPEED ) {
		_vec = _vec.normalize( ) * MOVE_SPEED;
	}
	if ( _vec.getLength( ) < 0.01 ) {
		_finished = true;
	}

	//クリスタル
	Vector crystal_vec = pos - _crystal.pos - Vector( 0, 0, FOOT );
	if ( crystal_vec.getLength( ) > MOVE_SPEED * 1.5 ) {
		crystal_vec = crystal_vec.normalize( ) * MOVE_SPEED * 1.5;
	}
	_crystal.pos += crystal_vec;
}

bool Delivery::isFinished( ) const {
	return _finished;
}

const Vector& Delivery::getPos( ) const {
	return _animation->getPos( );
}

void Delivery::setCrystal( Vector crystal_pos ) {
	if ( _have_crystal ) {
		return;
	}
	_have_crystal = true;
	_crystal.pos = crystal_pos;
	Vector pos = _animation->getPos( );

	while ( pos.x - crystal_pos.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.x - crystal_pos.x < -STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.y - crystal_pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	while ( pos.y - crystal_pos.y < -STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}

	_animation->setPos( pos );
	_target = _crystal.pos + Vector( 0, 0, FOOT );
	_crystal.type = MDL_CRYSTAL;
	_state = STATE_CATCH;
}

bool Delivery::isHaveCrystal( ) {
	return _have_crystal;
}