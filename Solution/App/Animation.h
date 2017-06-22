#pragma once
#include "Stage.h"

PTR( Animation );

class Animation {
public:
	enum ANIM {
		ANIM_DELIVERY_STAND,
		ANIM_DELIVERY_CATCH,
		ANIM_DELIVERY_CARRY,
		MAX_ANIM,
	};
public:
	Animation( ANIM anim );
	virtual ~Animation( );
	void update( );
	void changeAnim( ANIM anim );
	void setPos( Vector& pos );
	ANIM getAnim( ) const;
	const Vector& getPos( ) const;
	const Stage::MV1_INFO& getModel( ) const;
private:
	void updateAnimationDelivery( );
private:
	ANIM _anim;
	Stage::MV1_INFO _mv1;
};

