#include "Editor.h"
#include "EditorCamera.h"
#include "EditorStage.h"
#include "Application.h"
#include "Drawer.h"
#include "define.h"
#include "Stage.h"
#include "Keyboard.h"

static const Vector cursor_size = Vector( WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE );

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

	Matrix delivery_scale = Matrix::makeTransformScaling( DELIVERY_SIZE );
	drawer->loadMDLModel( MDL_DELIVERY, "Model/Delivery/delivery.mdl", "Model/Delivery/blue.jpg", delivery_scale );

	Matrix crystal_scale = Matrix::makeTransformScaling( CRYSTAL_SIZE );
	drawer->loadMDLModel( MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", crystal_scale );
	
	Matrix earth_scale = Matrix::makeTransformScaling( EARTH_SIZE );
	drawer->loadMDLModel( MDL_EARTH, "Model/Stage/earth.mdl", "Model/Stage/earth.jpg", earth_scale );
	
	Matrix cursor_scale = Matrix::makeTransformScaling( cursor_size );
	drawer->loadMDLModel( MDL_CURSOR, "Model/Editor/cursor.mdl", "Model/Editor/red.jpg", cursor_scale );

	drawer->loadMV1Model( MV1_WALL_0_1 , "Model/Stage/0_1.mdl"  );
	drawer->loadMV1Model( MV1_WALL_0_2 , "Model/Stage/0_2.mdl"  );
	drawer->loadMV1Model( MV1_WALL_0_3 , "Model/Stage/0_3.mdl"  );
	drawer->loadMV1Model( MV1_WALL_0_4 , "Model/Stage/0_4.mdl"  );
	drawer->loadMV1Model( MV1_WALL_0_5 , "Model/Stage/0_5.mdl"  );
	drawer->loadMV1Model( MV1_WALL_0_8 , "Model/Stage/0_8.mdl"  );
	drawer->loadMV1Model( MV1_WALL_0_10, "Model/Stage/0_10.mdl" );
	drawer->loadMV1Model( MV1_WALL_0_12, "Model/Stage/0_12.mdl" );
	drawer->loadMV1Model( MV1_WALL_0_15, "Model/Stage/0_15.mdl" );

	drawer->loadMV1Model( MV1_WALL_1_0 , "Model/Stage/1_0.mdl"  );
	drawer->loadMV1Model( MV1_WALL_1_1 , "Model/Stage/1_1.mdl"  );
	drawer->loadMV1Model( MV1_WALL_1_2 , "Model/Stage/1_2.mdl"  );
	drawer->loadMV1Model( MV1_WALL_1_3 , "Model/Stage/1_3.mdl"  );
	drawer->loadMV1Model( MV1_WALL_1_4 , "Model/Stage/1_4.mdl"  );
	drawer->loadMV1Model( MV1_WALL_1_5 , "Model/Stage/1_5.mdl"  );
	drawer->loadMV1Model( MV1_WALL_1_8 , "Model/Stage/1_8.mdl"  );
	drawer->loadMV1Model( MV1_WALL_1_10, "Model/Stage/1_10.mdl" );
	drawer->loadMV1Model( MV1_WALL_1_12, "Model/Stage/1_12.mdl" );
	drawer->loadMV1Model( MV1_WALL_1_15, "Model/Stage/1_15.mdl" );


	_camera = CameraPtr( new EditorCamera );
	_stage = StagePtr( new EditorStage( _camera ) );
}

void Editor::update( ) {
	_camera->update( );
	_stage->update( _camera );
	_stage->draw( );
}