#pragma once
#include "mathmatics.h"

const double REFLECTION_POWER = 0.25;
const int STAGE_WIDTH_NUM = 40;
const int STAGE_HEIGHT_NUM = 35;
const int FLOOR_CHIP_SIZE = 1;
const int WALL_DIV_SIZE = 4;//save����
const double WORLD_SCALE = 2;
const double ROOMBA_SCALE = 0.4;
const double STAGE_MODEL_SIZE = 4;
const double FLOOR_POS_Z = WORLD_SCALE;
const double BALL_RADIUS = WORLD_SCALE * ROOMBA_SCALE / 2;
const double GRAVITY = 0.1;
const Vector DELIVERY_SIZE = Vector( WORLD_SCALE / 4, WORLD_SCALE / 4, WORLD_SCALE / 4 ); 
const Vector CRYSTAL_SIZE = Vector( WORLD_SCALE / 4, WORLD_SCALE / 4, WORLD_SCALE / 4 );
const Vector BALL_SIZE = Vector( WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE );
const Vector FLOOR_SIZE = Vector( WORLD_SCALE, WORLD_SCALE, 1 );
const Vector WALL_SIZE = Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE / 2 * 3 );
const Vector EFFECT_ROTATE = Vector( PI / 2, 0, 0 );

enum MV1 {
	MV1_NONE,
	MV1_BALL,

	MV1_DELIVERY_STAND,
	MV1_DELIVERY_CATCH,
	MV1_DELIVERY_CARRY,
};

enum MDL {
	MDL_NONE,
	MDL_CURSOR,
	MDL_CRYSTAL,

	//editor
	MDL_WALL_0_0,
	MDL_WALL_0_1,
	MDL_WALL_0_2,
	MDL_WALL_0_3,
	MDL_WALL_0_4,
	MDL_WALL_0_5,
	MDL_WALL_0_6,
	MDL_WALL_0_7,
	MDL_WALL_0_8,
	MDL_WALL_0_9,
	MDL_WALL_0_10,
	MDL_WALL_0_11,
	MDL_WALL_0_12,
	MDL_WALL_0_13,
	MDL_WALL_0_14,
	MDL_WALL_0_15,

	MDL_WALL_1_0,
	MDL_WALL_1_1,
	MDL_WALL_1_2,
	MDL_WALL_1_3,
	MDL_WALL_1_4,
	MDL_WALL_1_5,
	MDL_WALL_1_6,
	MDL_WALL_1_7,
	MDL_WALL_1_8,
	MDL_WALL_1_9,
	MDL_WALL_1_10,
	MDL_WALL_1_11,
	MDL_WALL_1_12,
	MDL_WALL_1_13,
	MDL_WALL_1_14,
	MDL_WALL_1_15,
	MDL_FLOOR,
	MAX_MDL
};

enum GRAPH {
	GRAPH_NUMBER,
	GRAPH_PHASE,
	GRAPH_SELECT_MENU,
	GRAPH_GAME_OVER,
	GRAPH_GAME_CLEAR,
	GRAPH_RESULT_FRAME,
	GRAPH_FRAME,
	GRAPH_TITLE,
	GRAPH_RESULT,
	GRAPH_MAP,
	GRAPH_CIRCLE,
	GRAPH_FADE,
	GRAPH_FADE_BG,
	GRAPH_STAGE_SELECT,
	GRAPH_NUM_1,
	GRAPH_NUM_2,
	GRAPH_NUM_3,
	GRAPH_PLEASE_PUSH_BUTTON,
	GRAPH_LINK_GAUGE,
	GRAPH_LASER,
	GRAPH_MATRIX,
	GRAPH_MATRIX_ERASE,
	GRAPH_GUIDELINE,
	GRAPH_COMMAND_PROMPT_STRING,
	GRAPH_COMMAND_PROMPT_BACK,
	GRAPH_RANK,
	GRAPH_SHADOW,
	GRAPH_RADAR,
	GRAPH_WALL,
	GRAPH_FLOOR,
	MAX_GRAPH
};

enum EFFECT {
	EFFECT_CATCH_CRYSTAL,
	EFFECT_COLLISION_TO_WALL,
	EFFECT_COLLISION_TO_CRYSTAL,
	EFFECT_CRYSTAL_AURA,
	EFFECT_CHANGE_ROOMBA_STATE,
	EFFECT_DELIVERY_POINT,
	EFFECT_REBOOT,
};