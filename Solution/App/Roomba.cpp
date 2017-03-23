#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"

const double GRAVITY = -0.1;
const double SCALING_SPEED = 0.1;
static const Vector ROOMBA_SCALE( 2, 2, 2 );
static const double ACCEL = 0.1;
static const double ROTE_ACCEL = PI / 360;
static const double MAX_ROTE_SPEED = PI / 30;
static const double MAX_SPEED = 1.5;
static const double ROTE_SPEED = 0.1;

Roomba::Roomba( ) :
_range( 5 ),
_pos( 4, 0, 2 ),
_dir( 0, 1, 0 ),
_rote_speed( 0 ),
_state( STATE::STATE_NEUTRAL ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadMV1Model( MV1::MV1_BALL, "Model/Roomba/source/sphere.mv1" );
}


Roomba::~Roomba( ) {
}

void Roomba::update( StagePtr stage ) {
	_attack = false;
	move( );
	if ( _vec.getLength( ) > MAX_SPEED ) {
	//	_vec = _vec.normalize( ) * MAX_SPEED;
		_attack = true;
	}
	if ( isCollision( stage ) ) {
		_pos += _vec;
	}
}

void Roomba::move( ) {
	switch( _state ){
	case STATE::STATE_NEUTRAL:
		neutral( );
		break;
		//ステート変更
	case STATE::STATE_TRANSLATION:
		translation( );
		break;
	case STATE::STATE_ROTETION_SIDE:
		rotetionSide( );
		break;
	case STATE::STATE_ROTETION_BOTH:
		rotetionBoth( );
		break;
	}
}

void Roomba::neutral( ) {
	decelerationTranslation( );
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
	_range -= SCALING_SPEED;
	if ( _range < ROOMBA_SCALE.x ) {
		_range = ROOMBA_SCALE.x;
	}
	//回転
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), _rote_speed );
	_dir = mat.multiply( _dir );
}

void Roomba::translation( ) {
	decelerationRotetion( );
	bool hold = false;
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) && keyboard->isHoldKey( "W" ) ) {
		hold = true;
		_vec.y += ACCEL;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "S" ) ) {
		hold = true;
		_vec.y -= ACCEL;
	}

	_range -= SCALING_SPEED;
	if ( _range < ROOMBA_SCALE.x ) {
		_range = ROOMBA_SCALE.x;
	}
	if ( !hold ) {
		_state = STATE_NEUTRAL;
	}

}

void Roomba::rotetionSide( ) {
	bool hold = false;
	BALL move_type = BALL::BALL_LEFT;
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( ( keyboard->isHoldKey( "ARROW_UP" ) &&  keyboard->isHoldKey( "S" ) ) ||
		 ( keyboard->isHoldKey( "ARROW_DOWN" ) &&  keyboard->isHoldKey( "W" ) ) ) {
		_state = STATE_ROTETION_BOTH;
	}
	if ( ( keyboard->isHoldKey( "W" ) && keyboard->isHoldKey( "ARROW_UP" ) ) ||
		 ( keyboard->isHoldKey( "S" ) && keyboard->isHoldKey( "ARROW_DOWN" ) ) ) {
		_state = STATE_TRANSLATION;
	}
	if ( keyboard->isHoldKey( "S" ) && keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		_state = STATE_TRANSLATION;
	}
	//押しているか確かめる
	if ( keyboard->isHoldKey( "ARROW_UP" ) ||
		 keyboard->isHoldKey( "ARROW_DOWN" ) ||
		 keyboard->isHoldKey( "S" ) ||
		 keyboard->isHoldKey( "W" ) ) {
		hold = true;
	}
	//押してなかったらニュートラル
	if ( !hold ) {
		_state = STATE_NEUTRAL;
		return;
	}
	//回転速度変化
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		move_type = BALL::BALL_RIGHT;
		_rote_speed -= ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		move_type = BALL::BALL_RIGHT;
		_rote_speed += ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "S" ) ) {
		_rote_speed -= ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "W" ) ) {
		_rote_speed += ROTE_ACCEL;
	}

	_range += SCALING_SPEED;
	//回転速度max
	if ( _rote_speed > MAX_ROTE_SPEED ) {
		_rote_speed = MAX_ROTE_SPEED;
		_attack = true;
	}
	if ( _rote_speed < -MAX_ROTE_SPEED ) {
		_rote_speed = -MAX_ROTE_SPEED;
		_attack = true;
	}
	//回転
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), _rote_speed );
	Vector pos_before = convertToBallPos( move_type );
	_dir = mat.multiply( _dir );
	Vector pos_after = convertToBallPos( move_type );
	Vector vec = pos_after - pos_before;
	_vec = vec;
}

void Roomba::rotetionBoth( ) {
	bool hold = false;
	decelerationTranslation( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	//押しているか確かめる
	if ( ( keyboard->isHoldKey( "ARROW_UP" ) &&  keyboard->isHoldKey( "S" ) ) ||
		 ( keyboard->isHoldKey( "ARROW_DOWN" ) &&  keyboard->isHoldKey( "W" ) ) ) {
		hold = true;
	}
	//押してなかったらニュートラル
	if ( !hold ) {
		_state = STATE_NEUTRAL;
		return;
	}
	//回転速度変化
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		_rote_speed -= ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		_rote_speed += ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "S" ) ) {
		_rote_speed -= ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "W" ) ) {
		_rote_speed += ROTE_ACCEL;
	}
	//回転速度max
	if ( _rote_speed > MAX_ROTE_SPEED ) {
		_rote_speed = MAX_ROTE_SPEED;
		_attack = true;
	}
	if ( _rote_speed < -MAX_ROTE_SPEED ) {
		_rote_speed = -MAX_ROTE_SPEED;
		_attack = true;
	}
	_range += SCALING_SPEED;
	//回転
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), _rote_speed );
	_dir = mat.multiply( _dir );

}

void Roomba::decelerationTranslation( ) {
	//減速
	if ( _vec.x > 0 ) {
		_vec.x -= ACCEL;
		if ( _vec.x < 0 ) {
			_vec.x = 0;
		}
	}
	if ( _vec.y > 0 ) {
		_vec.y -= ACCEL;
		if ( _vec.y < 0 ) {
			_vec.y = 0;
		}
	}
	if ( _vec.x < 0 ) {
		_vec.x += ACCEL;
		if ( _vec.x > 0 ) {
			_vec.x = 0;
		}
	}
	if ( _vec.y < 0 ) {
		_vec.y += ACCEL;
		if ( _vec.y > 0 ) {
			_vec.y = 0;
		}
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


Vector Roomba::convertToBallPos( BALL type ) const {
	Matrix mat_ball = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector dir_ball = mat_ball.multiply( _dir );
	double v = 1 - 2 * ( type == BALL_LEFT );
	Vector pos = _pos + dir_ball.normalize( ) * _range * v;
	return pos;
}


void Roomba::draw( ) const {
	Vector pos[ 2 ];
	for ( int i = 0; i < MAX_BALL; i++ ) {
		pos[ i ] = convertToBallPos( (BALL)i );
	}

	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < MAX_BALL; i++ ) {
		Matrix mat = Matrix::makeTransformTranslation( pos[ i ] );
		Drawer::ModelMV1 model = Drawer::ModelMV1( mat, MV1::MV1_BALL, 0, 0 );
		drawer->setModelMV1( model );
	}
	if ( _attack ) {
		drawer->drawLine( pos[ 0 ], pos[ 1 ] );
	}
}

Vector Roomba::getPos( ) const {
	return _pos;
}

bool Roomba::isCollision( StagePtr stage ) {
	if ( stage->isCollisionWall( convertToBallPos( BALL_LEFT ) ) ||
		 stage->isCollisionWall( convertToBallPos( BALL_RIGHT ) ) ) {
		return false;
	}
	return true;
}