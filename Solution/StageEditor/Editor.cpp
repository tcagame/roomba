#include "Editor.h"
#include "EditorCamera.h"
#include "EditorStage.h"
#include "Application.h"
#include "Drawer.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"

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
	Matrix station_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE, WORLD_SCALE, WORLD_SCALE ) ); 
	drawer->loadMDLModel( MDL_STATION, "Model/Station/station.mdl", "Model/Station/purple.jpg", station_size );
	//drawer->loadMDLModel( MDL_STATION_1, "Model/Station/station.mdl", "Model/Station/green.jpg", station_size );
	//drawer->loadMDLModel( MDL_STATION_2, "Model/Station/station.mdl", "Model/Station/blue.jpg", station_size );
	//drawer->loadMDLModel( MDL_STATION_3, "Model/Station/station.mdl", "Model/Station/red.jpg", station_size );
	//drawer->loadMDLModel( MDL_STATION_4, "Model/Station/station.mdl", "Model/Station/yellow.jpg", station_size );

	Matrix crystal_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE / 2 ) ); 
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/purple.jpg", crystal_size );
	//drawer->loadMDLModel( MDL_CRYSTAL_1, "Model/Crystal/crystal.mdl", "Model/Crystal/green.jpg", crystal_size );
	//drawer->loadMDLModel( MDL_CRYSTAL_2, "Model/Crystal/crystal.mdl", "Model/Crystal/blue.jpg", crystal_size );
	//drawer->loadMDLModel( MDL_CRYSTAL_3, "Model/Crystal/crystal.mdl", "Model/Crystal/red.jpg", crystal_size );
	//drawer->loadMDLModel( MDL_CRYSTAL_4, "Model/Crystal/crystal.mdl", "Model/Crystal/yellow.jpg", crystal_size );
	
	Matrix earth_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / STAGE_MODEL_SIZE * STAGE_WIDTH_NUM, WORLD_SCALE / STAGE_MODEL_SIZE * STAGE_HEIGHT_NUM, WORLD_SCALE / STAGE_MODEL_SIZE ) );
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", earth_size );
	
	Matrix stage_size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE ) );
	drawer->loadMDLModel( MDL_CURSOR, "Model/Editor/cursor.mdl", "Model/Editor/red.jpg", stage_size );

	drawer->loadMDLModel( MDL_WALL_0_0 , "Model/Stage/0_0.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_1 , "Model/Stage/0_1.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_2 , "Model/Stage/0_2.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_3 , "Model/Stage/0_3.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_4 , "Model/Stage/0_4.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_5 , "Model/Stage/0_5.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_6 , "Model/Stage/0_6.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_7 , "Model/Stage/0_7.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_8 , "Model/Stage/0_8.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_9 , "Model/Stage/0_9.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_10, "Model/Stage/0_10.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_11, "Model/Stage/0_11.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_12, "Model/Stage/0_12.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_13, "Model/Stage/0_13.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_14, "Model/Stage/0_14.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_0_15, "Model/Stage/0_15.mdl", "Model/Stage/wall.jpg", stage_size );

	drawer->loadMDLModel( MDL_WALL_1_0 , "Model/Stage/1_0.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_1 , "Model/Stage/1_1.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_2 , "Model/Stage/1_2.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_3 , "Model/Stage/1_3.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_4 , "Model/Stage/1_4.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_5 , "Model/Stage/1_5.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_6 , "Model/Stage/1_6.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_7 , "Model/Stage/1_7.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_8 , "Model/Stage/1_8.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_9 , "Model/Stage/1_9.mdl" , "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_10, "Model/Stage/1_10.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_11, "Model/Stage/1_11.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_12, "Model/Stage/1_12.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_13, "Model/Stage/1_13.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_14, "Model/Stage/1_14.mdl", "Model/Stage/wall.jpg", stage_size );
	drawer->loadMDLModel( MDL_WALL_1_15, "Model/Stage/1_15.mdl", "Model/Stage/wall.jpg", stage_size );


	_camera = CameraPtr( new EditorCamera );
	_stage = StagePtr( new EditorStage );
}

void Editor::update( ) {
	_camera->update( );
	_stage->update( );
	_stage->draw( );
}