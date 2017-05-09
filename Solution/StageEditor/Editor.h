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
	EditorCameraPtr _camera;
	StagePtr _stage;
};

