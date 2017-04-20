#include "Task.h"
#include <string>

PTR( Game );
PTR( Camera );
PTR( Roomba );
PTR( Stage );
PTR( Timer );

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
	enum STATE {
		STATE_NORMAL,
		STATE_GAME_CLEAR,
		STATE_SELECT_RETRY,
		MAX_STATE,
	};
private:
	void drawResult( );
private:
	CameraPtr _camera;
	RoombaPtr _roomba;
	StagePtr _stage;
	TimerPtr _timer;
	STATE _state;
	char _select;
};