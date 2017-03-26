#pragma once
#include <ctime>

class Timer {
public:
	Timer( );
	virtual ~Timer( );
public:
	void draw( ) const;
	void addTime( );
	bool isTimeOver( ) const;
private:
	int _timer;
	time_t _watch;
};

