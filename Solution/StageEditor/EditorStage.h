#pragma once
#include "Stage.h"
#include "Editor.h"
#include "define.h"

class EditorStage : public Stage {
public:
	EditorStage( CameraPtr camera );
	virtual ~EditorStage( );
public:
	void update( CameraPtr camera );
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
	void editDelivery( );
private:
	void updateCursor( );
	void updateMode( );
	void drawCrystal( ) const;
	void drawFloor( ) const;
	void drawWall( ) const;
	void drawDelivery( ) const;
private:
	Vector _cursor_pos;
	int _count;
	Vector _before_mouse_pos;
	EDIT_MODE _mode;
	std::array< Drawer::ModelMV1, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > _floor;
	CameraPtr _camera;
};

