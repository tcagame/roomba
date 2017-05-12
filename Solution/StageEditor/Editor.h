#pragma once
#include <string>
#include "Task.h"

PTR( Editor );
PTR( Stage );
PTR( Camera );

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
	CameraPtr _camera;
	StagePtr _stage;
};

