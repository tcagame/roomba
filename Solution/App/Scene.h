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
protected:
	void drawFadeIn( ) const;
	void drawFadeOut( ) const;
	void addFadeInCount( );
	void subFadeOutCount( );
	int getFadeInCount( ) const;
	int getFadeOutCount( ) const;
	void resetCount( );
public:
	const int ANIME_FLAME = 2;
	const int MAX_FADE_COUNT = 34 * ANIME_FLAME;
private:
	int _fade_in_count;
	int _fade_out_count;
};

