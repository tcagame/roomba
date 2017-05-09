#pragma once
#include "Camera.h"

class EditorCamera : public Camera {
public:
	EditorCamera( );
	virtual ~EditorCamera( );
private:
	void move( );
};

