#include "Animation.h"

static const int MAX_ANIM_TIME[ Animation::ANIM::MAX_ANIM ] = {
	31,
	31,
	30
};



Animation::Animation( ANIM anim ) {
	_mv1.pos = Vector( );
	_mv1.model = Drawer::ModelMV1( Matrix( ), MV1_NONE, 0, 0 );
	changeAnim( anim );
}


Animation::~Animation( ) {
}

void Animation::update( ) {
	updateAnimationDelivery( );
}

void Animation::draw( ViewerConstPtr viewer ) {
	Matrix scale = Matrix::makeTransformScaling( DELIVERY_SIZE );
	Matrix rot = Matrix::makeTransformRotation( Vector( 1, 0, 0 ), PI / 2 );
	Matrix scale_rot = scale.multiply( rot );
	{
		Vector vec = _mv1.pos - _old_pos;
		vec.z = 0;
		Vector vec_diff = vec - _old_vec;
		Vector rot_vec = vec_diff + vec * -1.5;
		Matrix rot2 = Matrix::makeTransformRotation( rot_vec.normalize( ), rot_vec.getLength( ) );
		scale_rot = scale_rot.multiply( rot2 );
		
		_old_vec = vec;
		_old_pos = _mv1.pos;
	}
	if ( viewer ) {
		viewer->drawModelMV1( _mv1, scale_rot );
	} else {
		Matrix trans = Matrix::makeTransformTranslation( _mv1.pos );
		_mv1.model.matrix = scale_rot.multiply( trans );
		DrawerPtr drawer = Drawer::getTask( );
		drawer->setModelMV1( _mv1.model );
	}
}


void Animation::updateAnimationDelivery( ) {
	_mv1.model.time += 1;
	switch ( _anim ) {
	case ANIM_DELIVERY_STAND:
		_mv1.model.time = (int)_mv1.model.time % MAX_ANIM_TIME[ ANIM_DELIVERY_STAND ];
		break;
	case ANIM_DELIVERY_CATCH:
		if ( (int)_mv1.model.time > MAX_ANIM_TIME[ ANIM_DELIVERY_CATCH ] ) {
			changeAnim( ANIM_DELIVERY_CARRY );
		}
		break;
	case ANIM_DELIVERY_CARRY:
		_mv1.model.time = (int)_mv1.model.time % MAX_ANIM_TIME[ ANIM_DELIVERY_CARRY ];
		break;
	}
}

void Animation::changeAnim( ANIM anim ) {
	_anim = anim;
	_mv1.model.time = 0;
	switch ( _anim ) {
	case ANIM_DELIVERY_STAND:
		_mv1.model.mesh = MV1_DELIVERY_STAND;
		break;
	case ANIM_DELIVERY_CATCH:
		_mv1.model.mesh = MV1_DELIVERY_CATCH;
		break;
	case ANIM_DELIVERY_CARRY:
		_mv1.model.mesh = MV1_DELIVERY_CARRY;
		break;
	}
}

void Animation::setPos( Vector& pos ) {
	_mv1.pos = pos;
}

Animation::ANIM Animation::getAnim( ) const {
	return _anim;
}

const Stage::MV1_INFO& Animation::getModel( ) const {
	return _mv1;
}

const Vector& Animation::getPos( ) const {
	return _mv1.pos;
}