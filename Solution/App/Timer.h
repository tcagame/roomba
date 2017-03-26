#pragma once

class Timer {
public:
	Timer( );
	virtual ~Timer( );
public:
	void update( );
	void draw( ) const;
	bool isTimeOver( ) const;
private:
	int _timer;
};

