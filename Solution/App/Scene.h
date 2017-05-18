#pragma once

class Scene {
public:
	enum NEXT {
		NEXT_CONTINUE,
		NEXT_STAGE_SELECT,
		NEXT_TITLE,
		NEXT_STAGE,
		NEXT_RESULT,
		NEXT_RETRY,
		NEXT_DEBUG_RESULT,
	};
public:
	Scene( );
	virtual ~Scene( );
	virtual NEXT update( ) = 0;
};

