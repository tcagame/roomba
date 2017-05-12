#pragma once
#include "Stage.h"
#include "Editor.h"

class EditorStage : public Stage {
public:
	EditorStage( );
	virtual ~EditorStage( );
public:
	void update( );
	void draw( ) const;
private:
	enum EDIT_MODE {
		EDIT_MODE_WALL,
		EDIT_MODE_CRYSTAL,
		EDIT_MODE_STATION,
		EDIT_MODE_MAX
	};
private:
	void saveFile( ) const;
	void loadFile( );
	void edit( );
	void editWall( );
	void editCrystal( );
	void editStation( );
private:
	void updateCursor( );
	void updateMode( );
	void drawCrystal( ) const;
private:
	int _cursor_x;
	int _cursor_y;
	int _count;
	EDIT_MODE _mode;
};

