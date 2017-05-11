#pragma once
#include <string>
#include "Task.h"

PTR( Editor );
PTR( EditorCamera );
PTR( Stage );

class Editor : public Task {
public:
	static std::string getTag( ) { return "EDITOR"; };
	static EditorPtr getTask( );
public:
	Editor( );
	virtual ~Editor( );
public:
	void initialize( );
	void update( );
private:
	enum MODE {
		MODE_WALL,
		MODE_CRYSTAL,
		MODE_STATION,
		MODE_NONE
	};
private:
	void updateMode( );
	void updateStage( );
private:
	EditorCameraPtr _camera;
	StagePtr _stage;
	MODE _mode;
};

