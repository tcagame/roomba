#pragma once
#include "mathmatics.h"

const double REFLECTION_POWER = 0.25;
const int STAGE_WIDTH_NUM = 40;
const int STAGE_HEIGHT_NUM = 35;
const double WORLD_SCALE = 2;
const double ROOMBA_SCALE = 0.4;
const double STAGE_MODEL_SIZE = 4;
const double EARTH_POS_Z = WORLD_SCALE;
const double BALL_RADIUS = WORLD_SCALE * ROOMBA_SCALE / 2;
const double GRAVITY = 0.1;
const Vector DELIVERY_SIZE = Vector( WORLD_SCALE / 4, WORLD_SCALE / 4, WORLD_SCALE / 4 ); 
const Vector CRYSTAL_SIZE = Vector( WORLD_SCALE / 4, WORLD_SCALE / 4, WORLD_SCALE / 4 );
const Vector BALL_SIZE = Vector( WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE );
const Vector EARTH_SIZE = Vector( WORLD_SCALE / STAGE_MODEL_SIZE * STAGE_WIDTH_NUM, WORLD_SCALE / STAGE_MODEL_SIZE * STAGE_HEIGHT_NUM, WORLD_SCALE / STAGE_MODEL_SIZE );
const Vector WALL_SIZE = Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE / 2 * 3 );
const Vector EFFECT_ROTATE = Vector( PI / 2, 0, 0 );

enum MV1 {
	MV1_NONE,
	MV1_BALL,
	MV1_DELIVERY,
	MV1_WALL_0_0,
	MV1_WALL_0_1,
	MV1_WALL_0_2,
	MV1_WALL_0_3,
	MV1_WALL_0_4,
	MV1_WALL_0_5,
	MV1_WALL_0_6,
	MV1_WALL_0_7,
	MV1_WALL_0_8,
	MV1_WALL_0_9,
	MV1_WALL_0_10,
	MV1_WALL_0_11,
	MV1_WALL_0_12,
	MV1_WALL_0_13,
	MV1_WALL_0_14,
	MV1_WALL_0_15,

	MV1_WALL_1_0,
	MV1_WALL_1_1,
	MV1_WALL_1_2,
	MV1_WALL_1_3,
	MV1_WALL_1_4,
	MV1_WALL_1_5,
	MV1_WALL_1_6,
	MV1_WALL_1_7,
	MV1_WALL_1_8,
	MV1_WALL_1_9,
	MV1_WALL_1_10,
	MV1_WALL_1_11,
	MV1_WALL_1_12,
	MV1_WALL_1_13,
	MV1_WALL_1_14,
	MV1_WALL_1_15,
};

enum MDL {
	MDL_NONE,
	MDL_EARTH,
	MDL_CURSOR,
	MDL_CRYSTAL,
	MAX_MDL
};

enum GRAPH {
	GRAPH_NUMBER,
	GRAPH_PHASE,
	GRAPH_SELECT_MENU,
	GRAPH_GAMEOVER,
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
	GRAPH_READY,
	GRAPH_GUIDELINE,
	GRAPH_COMMAND_PROMPT_STRING,
	GRAPH_COMMAND_PROMPT_BACK,
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