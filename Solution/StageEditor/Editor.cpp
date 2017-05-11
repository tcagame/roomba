#include "Editor.h"
#include "EditorCamera.h"
#include "Application.h"
#include "Drawer.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"

static const double STAGE_MODEL_SIZE = 4;


EditorPtr Editor::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Editor >( app->getTask( getTag( ) ) );
}

Editor::Editor( ) {
}


Editor::~Editor( ) {

}

void Editor::initialize( ) {
	//drawer‰Šú‰»Œã
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_TIMER_NUM, "UI/timenumber.png" );
	drawer->loadGraph( GRAPH_SELECT_MENU, "UI/UI_retry_select.png" );
	
	Matrix size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE / 2 ) ); 
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", size );
	drawer->loadMDLModel( MDL_BG, "Model/Stage/bg.mdl", "Model/Stage/bg01_DM.jpg" );
	drawer->loadMDLModel( MDL_BALL, "Model/Roomba/roomba.mdl", "Model/Roomba/texture.jpg", size );
	
	size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE ) );
	
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", size );
	drawer->loadMDLModel( MDL_CURSOR, "Model/Editor/cursor.mdl", "Model/Editor/red.jpg", size );

	drawer->loadMDLModel( MDL_WALL_0_0 , "Model/Stage/0_0.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_1 , "Model/Stage/0_1.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_2 , "Model/Stage/0_2.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_3 , "Model/Stage/0_3.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_4 , "Model/Stage/0_4.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_5 , "Model/Stage/0_5.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_6 , "Model/Stage/0_6.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_7 , "Model/Stage/0_7.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_8 , "Model/Stage/0_8.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_9 , "Model/Stage/0_9.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_10, "Model/Stage/0_10.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_11, "Model/Stage/0_11.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_12, "Model/Stage/0_12.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_13, "Model/Stage/0_13.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_14, "Model/Stage/0_14.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_0_15, "Model/Stage/0_15.mdl", "Model/Stage/wall.jpg", size );

	drawer->loadMDLModel( MDL_WALL_1_0 , "Model/Stage/1_0.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_1 , "Model/Stage/1_1.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_2 , "Model/Stage/1_2.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_3 , "Model/Stage/1_3.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_4 , "Model/Stage/1_4.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_5 , "Model/Stage/1_5.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_6 , "Model/Stage/1_6.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_7 , "Model/Stage/1_7.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_8 , "Model/Stage/1_8.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_9 , "Model/Stage/1_9.mdl" , "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_10, "Model/Stage/1_10.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_11, "Model/Stage/1_11.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_12, "Model/Stage/1_12.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_13, "Model/Stage/1_13.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_14, "Model/Stage/1_14.mdl", "Model/Stage/wall.jpg", size );
	drawer->loadMDLModel( MDL_WALL_1_15, "Model/Stage/1_15.mdl", "Model/Stage/wall.jpg", size );


	_camera = EditorCameraPtr( new EditorCamera );
	_stage = StagePtr( new Stage );
}

void Editor::update( ) {
	_camera->update( );
	updateStage( );
	updateMode( );
}

void Editor::updateStage( ) {
	_stage->updateCursor( );
	_stage->draw( );
	_stage->drawEditor( );

	switch ( _mode ) {
	case MODE_WALL:
		_stage->editWall( );
		break;
	case MODE_CRYSTAL:
		_stage->editCrystal( );
		break;
	case MODE_STATION:
		_stage->editStation( );
		break;
	}

	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isPushKey( "F1" ) ) {
		_stage->load( );
	}
	if ( keyboard->isPushKey( "F2" ) ) {
		_stage->save( );
	}
	if ( keyboard->isPushKey( "NUM0" ) ) {
		_stage->setPhase( 0 );
	}
	if ( keyboard->isPushKey( "NUM1" ) ) {
		_stage->setPhase( 1 );
	}
	if ( keyboard->isPushKey( "NUM2" ) ) {
		_stage->setPhase( 2 );
	}
	if ( keyboard->isPushKey( "NUM3" ) ) {
		_stage->setPhase( 3 );
	}
	if ( keyboard->isPushKey( "NUM4" ) ) {
		_stage->setPhase( 4 );
	}
}

void Editor::updateMode( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isPushKey( "1" ) ) {
		_mode = MODE_WALL;
	}
	if ( keyboard->isPushKey( "2" ) ) {
		_mode = MODE_CRYSTAL;
	}
	if ( keyboard->isPushKey( "3" ) ) {
		_mode = MODE_STATION;
	}
}