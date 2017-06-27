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
	void setCollisionNum( int col_num );
	void setResult( bool clear );
	void setOpenStage( );
private:
	void changeScene( );
private:
	int _col_num;
	int _stage_num;
	int _result_time;
	bool _stage_clear;
	bool _open_stage;
	Scene::NEXT _next;
	ScenePtr _scene;
};