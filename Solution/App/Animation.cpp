#include "Animation.h"

static const int MAX_ANIM_TIME[ Animation::ANIM::MAX_ANIM ] = {
	31,
	31,
	30
};



Animation::Animation( ANIM anim ) {
	changeAnim( anim );
}


Animation::~Animation( ) {
}

void Animation::update( ) {
	updateAnimationDelivery( );
}

void Animation::updateAnimationDelivery( ) {
	_model.time++;
	switch ( _anim ) {
	case ANIM_DELIVERY_STAND:
		_model.time %= MAX_ANIM_TIME[ ANIM_DELIVERY_STAND ];
		break;
	case ANIM_DELIVERY_CATCH:
		if ( _model.time > MAX_ANIM_TIME[ ANIM_DELIVERY_CATCH ] ) {
			changeAnim( ANIM_DELIVERY_CARRY );
		}
		break;
	case ANIM_DELIVERY_CARRY:
		_model.time %= MAX_ANIM_TIME[ ANIM_DELIVERY_CARRY ];
		break;
	}
}

void Animation::changeAnim( ANIM anim ) {
	_anim = anim;
	_model.time = 0;
	switch ( _anim ) {
	case ANIM_DELIVERY_STAND:
		_model.type = MV1_DELIVERY_STAND;
		break;
	case ANIM_DELIVERY_CATCH:
		_model.type = MV1_DELIVERY_CATCH;
		break;
	case ANIM_DELIVERY_CARRY:
		_model.type = MV1_DELIVERY_CARRY;
		break;
	}
}

void Animation::setPos( Vector pos ) {
	_model.pos = pos;
}


Animation::ANIM Animation::getAnim( ) const {
	return _anim;
}

const Stage::MV1_INFO& Animation::getModel( ) const {
	return _model;
}
