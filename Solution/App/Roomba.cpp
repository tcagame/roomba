#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"
#include "Ball.h"

static const double ACCEL = 0.1;
const double SCALING_SPEED = 0.1;
static const Vector ROOMBA_SCALE( 2, 2, 2 );
static const double ROTE_ACCEL = PI / 360;
static const double MAX_ROTE_SPEED = PI / 180;
static const double MAX_SPEED = 0.5;
static const double ROTE_SPEED = 0.1;
static const Vector START_POS( 4, 0, 1 );
static const double CENTRIPETAL_RATIO = 0.3;
static const double CENTRIPETAL_MIN = 3.5;

Roomba::Roomba( ) :
_dir( 0, 1, 0 ),
_rote_speed( 0 ),
_state( STATE::STATE_NEUTRAL ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadMV1Model( MV1::MV1_BALL, "Model/Roomba/source/sphere.mv1" );
	_balls[ BALL_LEFT ] = BallPtr( new Ball( START_POS ) );
	_balls[ BALL_RIGHT ] = BallPtr( new Ball( START_POS + Vector( 10, 0, 0 ) ) );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage ) {
	//_attacking = false;
	//回転速度または、移動速度がMAXのときにtrueにする
	move( );
	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->update( stage );
	}

	//攻撃
	attack( stage );
}

void Roomba::move( ) {
	Vector vec[ MAX_BALL ];
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		vec[ BALL_RIGHT ] += _dir.normalize( ) * ACCEL;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		vec[ BALL_RIGHT ] -= _dir.normalize( ) * ACCEL;
	}
	if ( keyboard->isHoldKey( "W" ) ) {
		vec[ BALL_LEFT ] += _dir.normalize( ) * ACCEL;
	}
	if (  keyboard->isHoldKey( "S" ) ) {
		vec[ BALL_LEFT ] -= _dir.normalize( ) * ACCEL;
	}
	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->addAccel( vec[ i ] );
	}
	centripetal( );
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	_dir = mat.multiply( _balls[ BALL_LEFT ]->getPos( ) - _balls[ BALL_RIGHT ]->getPos( ) );
}

void Roomba::centripetal( ) {
	Vector vec[ MAX_BALL ];
	for ( int i = 0; i < MAX_BALL; i++ ) {
		vec[ i ] = getCentralPos( ) - _balls[ i ]->getPos( );
		if ( vec[ i ].getLength( ) < CENTRIPETAL_MIN ) {
			vec[ i ] = Vector( );
			continue;
		}
		vec[ i ] -= vec[ i ].normalize( ) * CENTRIPETAL_MIN;
		vec[ i ] *= CENTRIPETAL_RATIO;
	}
	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->addAccel( vec[ i ] );
	}
}

void Roomba::neutral( ) {
	decelerationRotetion( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		_state = STATE_ROTETION_SIDE;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		_state = STATE_ROTETION_SIDE;
	}
	if ( keyboard->isHoldKey( "W" ) ) {
		_state = STATE_ROTETION_SIDE;
	}
	if ( keyboard->isHoldKey( "S" ) ) {
		_state = STATE_ROTETION_SIDE;
	}
}

void Roomba::translation( ) {
	decelerationRotetion( );
	bool hold = false;
	KeyboardPtr keyboard = Keyboard::getTask( );
	Vector vec[ MAX_BALL ];
	if ( keyboard->isHoldKey( "ARROW_UP" ) && keyboard->isHoldKey( "W" ) ) {
		hold = true;
		for ( int i = 0; i < MAX_BALL; i++ ) {
			vec[ i ].y += ACCEL;
		}
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "S" ) ) {
		hold = true;
		for ( int i = 0; i < MAX_BALL; i++ ) {
			vec[ i ].y -= ACCEL;
		}
	}

	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->addAccel( vec[ i ] );
	}
}


void Roomba::decelerationRotetion( ) {
	if ( _rote_speed > 0 ) {
		_rote_speed -= ROTE_ACCEL / 6;
	if ( _rote_speed < 0 ) {
			_rote_speed = 0;
		}
	}
	if ( _rote_speed < 0 ) {
		_rote_speed += ROTE_ACCEL / 6;
		if ( _rote_speed > 0 ) {
			_rote_speed = 0;
		}
	}
}

void Roomba::attack( StagePtr stage ) {
	if ( !_attacking ) {
		return;
	}
	CrystalPtr crystal =  stage->getHittingCrystal( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
	if ( crystal ) {
		DrawerPtr drawer = Drawer::getTask( );
		drawer->drawString( 0, 0, "あたってるよー" );
	}
}

void Roomba::draw( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < MAX_BALL; i++ ) {
		_balls[ i ]->draw( );
	}

	if ( _attacking ) {
		drawer->drawLine( _balls[ BALL_LEFT ]->getPos( ), _balls[ BALL_RIGHT ]->getPos( ) );
	}
}

Vector Roomba::getCentralPos( ) const {
	Vector pos[ MAX_BALL ];
	for ( int i = 0; i < MAX_BALL; i++ ) {
		pos[ i ] = _balls[ i ]->getPos( );
	}
	Vector central_pos = ( pos[ BALL_LEFT ] + pos[ BALL_RIGHT ] ) * 0.5;

	return central_pos;
}


bool Roomba::isCollision( StagePtr stage ) {
	//保留
	//if ( stage->isCollisionWall( convertToBallPos( BALL_LEFT ) ) ||
	//	 stage->isCollisionWall( convertToBallPos( BALL_RIGHT ) ) ) {
	//	return false;
	//}
	return false;
}