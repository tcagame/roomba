#pragma once
#include "Task.h"
#include <string>

PTR( Game );
PTR( Camera );
PTR( Roomba );
PTR( Stage );

class Game : public Task {
public:
	static std::string getTag( ) { return "GAME"; };
	static GamePtr getTask( );
public:
	Game( );
	virtual ~Game( );
public:
	void update( );
	void initialize( );
private:
	CameraPtr _camera;
	RoombaPtr _Roomba;
	StagePtr _stage;
};