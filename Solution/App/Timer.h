#pragma once
#include <ctime>

class Timer {
public:
	Timer( );
	virtual ~Timer( );
public:
	void update( );
	void draw( ) const;
	void addTime( );
private:
	int _timer;
	time_t _watch;
};

