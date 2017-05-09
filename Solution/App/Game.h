#include "Task.h"
#include "Scene.h"
#include <string>
#include <list>

PTR( Game );
PTR( AppCamera );
PTR( Roomba );
PTR( Stage );
PTR( Timer );
PTR( Scene );

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
	void changeScene( );
private:
	ScenePtr _scene;
	AppCameraPtr _camera;
	RoombaPtr _roomba;
	StagePtr _stage;
	TimerPtr _timer;
	STATE _state;
	char _select;
	Scene::NEXT _next;
};