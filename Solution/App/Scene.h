#pragma once

class Scene {
public:
	enum NEXT {
		NEXT_CONTINUE,
		NEXT_STAGE_SELECT,
		NEXT_TITLE,
		NEXT_STAGE_1,
		NEXT_STAGE_2,
		NEXT_STAGE_3,
		NEXT_DEBUG_RESULT,
	};
public:
	Scene( );
	virtual ~Scene( );
	virtual NEXT update( ) = 0;
};

