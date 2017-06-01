#pragma once
#include "Camera.h"

class EditorCamera : public Camera {
public:
	EditorCamera( );
	virtual ~EditorCamera( );
private:
	void move( );
private:
	Vector _before_mouse_pos;
	double _length;
};

