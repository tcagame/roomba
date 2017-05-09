#pragma once

class Scene {
public:
	enum NEXT {
		NEXT_TITLE,
		NEXT_STAGE,
		NEXT_DEBUG_RESULT,
	};
public:
	Scene( );
	virtual ~Scene( );
	virtual NEXT update( ) = 0;
};

