#pragma once
#include "smart_ptr.h"
#include "Drawer.h"
#include <array>

PTR ( Viewer );
class RoombaDelivery {
public:
	RoombaDelivery( );
	virtual ~RoombaDelivery( );
public:
	void update( );
	void draw( ) const;
	void setPos( Vector ball_left, Vector ball_right );
	bool isRestart( ) const;
	bool isReady( ) const;
	bool isDrawRoomba( ) const;
private:
	enum  STATE {
		STATE_UP,
		STATE_DOWN,
	};
private:
	void changeState( );
	void chackDraw( );
	void liftUp( );
	void liftDown( );
private:
	STATE _state;
	int _pickup_count;
	bool _restart;
	bool _ready;
	bool _draw_roomba;
	std::array < Vector, 2 > _pos;
	Vector _vec;
};
