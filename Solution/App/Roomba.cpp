#include "Roomba.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"

const double GRAVITY = -0.1;
const double SCALING_SPEED = 0.1;
static const Vector ROOMBA_SCALE( 2, 2, 2 );
static const double ACCEL = 0.1;
static const double ROTE_ACCEL = PI / 360;
static const double MAX_ROTE_SPEED = PI / 15;
static const double MAX_SPEED = 0.6;
static const double ROTE_SPEED = 0.1;

Roomba::Roomba( ) :
_range( 5 ),
_pos( 4, 0, 10 ),
_dir( 1, 0, 0 ),
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
		_vec = _vec.normalize( ) * MAX_SPEED;
		_attack = true;
	}
	_pos += _vec;
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
	case STATE::STATE_ROTETION:
		rotetion( );
		break;
	}
}

void Roomba::neutral( ) {
	deceleration( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isHoldKey( "ARROW_UP" ) && keyboard->isHoldKey( "W" ) ) {
		_state = STATE_TRANSLATION;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "S" ) ) {
		_state = STATE_TRANSLATION;
	}
	if ( keyboard->isHoldKey( "ARROW_UP" ) && keyboard->isHoldKey( "S" ) ) {
		_state = STATE_ROTETION;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) && keyboard->isHoldKey( "W" ) ) {
		_state = STATE_ROTETION;
	}
	_range -= SCALING_SPEED;
	if ( _range < ROOMBA_SCALE.x ) {
		_range = ROOMBA_SCALE.x;
	}
	{//回転
		Matrix mat = Matrix( );
		Vector axis = Vector( 0, 0, 1 );
		mat = mat.makeTransformRotation( axis, _rote_speed );
		_dir = mat.multiply( _dir );
	}
}

void Roomba::translation( ) {
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

void Roomba::rotetion( ) {
	bool hold = false;
	bool synchro = false;
	KeyboardPtr keyboard = Keyboard::getTask( );
	//右ルンバ
	if ( keyboard->isHoldKey( "ARROW_UP" ) &&  keyboard->isHoldKey( "S" ) ) {
		synchro = true;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) &&  keyboard->isHoldKey( "W" ) ) {
		synchro = true;
	}
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		hold = true;
		_rote_speed -= ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		_rote_speed += ROTE_ACCEL;
		hold = true;
	}
	//左ルンバ
	if ( keyboard->isHoldKey( "S" ) ) {
		hold = true;
		_rote_speed -= ROTE_ACCEL;
	}
	if ( keyboard->isHoldKey( "W" ) ) {
		_rote_speed += ROTE_ACCEL;
		hold = true;
	}
	if ( _rote_speed > MAX_ROTE_SPEED ) {
		_rote_speed = MAX_ROTE_SPEED;
		_attack = true;
	}
	if ( _rote_speed < -MAX_ROTE_SPEED ) {
		_rote_speed = -MAX_ROTE_SPEED;
		_attack = true;
	}
	_range += SCALING_SPEED;
	if ( !hold ) {
		_state = STATE_NEUTRAL;
	}
	Matrix mat = Matrix( );
	Vector axis = Vector( 0, 0, 1 );
	mat = mat.makeTransformRotation( axis, _rote_speed );
	_dir = mat.multiply( _dir );
	if ( !synchro ) {
		////速度ベクトル？
		//_
		//Vector vec;// = pos1 - pos0;
		////行列回転
		//vec = mat.multiply( vec );
		////左ルンバ+vec.nom * range
		//Vector pos = pos0 + vec.normalize( ) * _range;
		////未来-現在
		//_vec.x = pos.x - _pos.x;
		//_vec.y = pos.y - _pos.y;
	}
}

void Roomba::deceleration( ) {
	//減速
	if ( _vec.x > 0 ) {
		if ( _vec.x < ACCEL ) {
			_vec.x = 0;
		}
		_vec.x -= ACCEL;
	}
	if ( _vec.y > 0 ) {
		if ( _vec.y < ACCEL ) {
			_vec.y = 0;
		}
		_vec.y -= ACCEL;
	}
	if ( _rote_speed > 0 ) {
		_rote_speed -= ROTE_ACCEL / 6;
		if ( _rote_speed < 0 ) {
			_rote_speed = 0;
		}
	}
	if ( _rote_speed< 0 ) {
		_rote_speed += ROTE_ACCEL / 6;
		if ( _rote_speed > 0 ) {
			_rote_speed = 0;
		}
	}
}

void Roomba::draw( ) const {
	Matrix mat_ball = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI / 2 );
	Vector dir_ball = mat_ball.multiply( _dir );
	Vector pos[ 2 ];
	for ( int i = 0; i < MAX_BALL; i++ ) {
		double v = 1 - 2 * ( (BALL)i == BALL_LEFT );
		pos[ i ] = _pos + dir_ball.normalize( ) * _range * v;
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