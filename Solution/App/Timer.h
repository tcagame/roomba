#pragma once
#include "Drawer.h"
#include <list>

class Timer {
public:
	Timer( );
	virtual ~Timer( );
public:
	void update( );
	void draw( ) const;
	void addTime( );
	void reset( );
	void finalize( );
	bool isTimeOver( ) const;
private:
	void updateRecovery( );
private:
	int _timer;
	double _size;
	std::list< int > _add_times;
};

