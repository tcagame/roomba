#include "Task.h"
#include "Scene.h"
#include <string>
#include <list>

PTR( Game );
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
	void setStage( int stage_num );
	void setResultTime( int time );
private:
	void changeScene( );
private:
	int _stage_num;
	int _result_time;
	Scene::NEXT _next;
	ScenePtr _scene;
};