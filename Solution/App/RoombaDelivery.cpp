#include "RoombaDelivery.h"
#include "define.h"
#include "Viewer.h"
#include "Roomba.h"

static const double SPEED = 0.1;
static const double MAX_SPEED = 0.2;
static const double FLAME_OUT_Z = 15;
static const double SETTING_Z = roomba_size.z * 0.5;

RoombaDelivery::RoombaDelivery( ):
_state( STATE_DOWN ),
_vec( Vector( ) ),
_pickup_count( 0 ),
_restart( false ),
_ready( false ),
_draw_roomba( true ) {
	for ( int i = 0; i < 2; i++ ) {
		_pos[ i ] = Vector( 0, 0, FLAME_OUT_Z );
	}
}


RoombaDelivery::~RoombaDelivery( ) {
}

void RoombaDelivery::update( ) {
	changeState( );
	chackDraw( );
	switch ( _state ) {
	case STATE_DOWN:
		liftDown( );
		break;
	case STATE_UP:
		liftUp( );
		break;
	}

	for ( int i = 0; i < 2; i++ ) {
		_pos[ i ] += _vec;
	}
}
	
void RoombaDelivery::draw( RoombaConstPtr roomba ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < 2; i++ ) {
		Vector delivery_pos = _pos[ i ] - Vector( 0, 0, -station_size.z );
		drawer->setModelMDL( Drawer::ModelMDL( delivery_pos, MDL_DELIVERY ) );
		if ( ( _pickup_count % 4 ) == 1 || ( _pickup_count % 4 ) == 2 ) {			
			Vector roomba_pos = _pos[ i ];
			if ( roomba_pos.z < roomba_size.z ) {
				roomba_pos.z = roomba_size.z;
			}
			//drawer->setModelMDL( Drawer::ModelMDL( roomba_pos, MDL_BALL ) );
			drawer->setModelMV1( Drawer::ModelMV1( roomba->getMat( i, roomba_pos ), 0, 0, 0 ) );
		}
	}
}

void RoombaDelivery::liftDown( ) {
	_vec.z -= SPEED;
	if ( _vec.z < -MAX_SPEED ) {
		_vec.z = -MAX_SPEED;
	}
}

void RoombaDelivery::liftUp( ) {
	_vec.z += SPEED;
	if ( _vec.z > MAX_SPEED ) {
		_vec.z = MAX_SPEED;
	}
}

void RoombaDelivery::setPos( Vector ball_left, Vector ball_right ) {
	_pos[ 0 ] = ball_left;
	_pos[ 1 ] = ball_right;
	_pos[ 0 ].z = FLAME_OUT_Z;
	_pos[ 1 ].z = FLAME_OUT_Z;
}

void RoombaDelivery::changeState( ) {
	_restart = false;
	_ready = false;
	STATE state = _state;
	if ( _pos[ 0 ].z + _vec.z < SETTING_Z + delivery_size.z ) {
		state = STATE_UP;
	}
	if ( _pos[ 0 ].z > FLAME_OUT_Z ) {
		state = STATE_DOWN;
	}
	if ( state != _state ) {
		_state = state;
		if ( ( _pickup_count % 4 ) == 1 ) {
			_restart = true;
		}
		if ( ( _pickup_count % 4 ) == 3 ) {
			_ready = true;
		}
		_pickup_count++;
	}
}

bool RoombaDelivery::isRestart( ) const {
	return _restart;
}

bool RoombaDelivery::isReady( ) const {
	return _ready;
}

bool RoombaDelivery::isDrawRoomba( ) const {
	return _draw_roomba;
}

void RoombaDelivery::chackDraw( ) {
	_draw_roomba = true;
	if ( ( _pickup_count % 4 ) == 1 ||
		 ( _pickup_count % 4 ) == 2 ) {
		_draw_roomba = false;
	}
}