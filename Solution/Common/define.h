#pragma once
#include "mathmatics.h"

static const int STAGE_WIDTH_NUM = 40;
static const int STAGE_HEIGHT_NUM = 35;
static const double WORLD_SCALE = 2;
static const double ROOMBA_SCALE = 0.1;
static const double STAGE_MODEL_SIZE = 4;
static const double EARTH_POS_Z = WORLD_SCALE;
static const double BALL_RADIUS = WORLD_SCALE * ROOMBA_SCALE * 0.8;
static const double GRAVITY = 0.1;
static const Vector delivery_size = Vector( WORLD_SCALE, WORLD_SCALE, WORLD_SCALE / 4 ); 
static const Vector crystal_size = Vector( WORLD_SCALE / 8, WORLD_SCALE / 8, WORLD_SCALE / 6 );
static const Vector roomba_size = Vector( WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE, WORLD_SCALE * ROOMBA_SCALE );
static const Vector earth_size = Vector( WORLD_SCALE / STAGE_MODEL_SIZE * STAGE_WIDTH_NUM, WORLD_SCALE / STAGE_MODEL_SIZE * STAGE_HEIGHT_NUM, WORLD_SCALE / STAGE_MODEL_SIZE );
static const Vector wall_size = Vector( WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE, WORLD_SCALE / STAGE_MODEL_SIZE * 3 );
static const Vector EFFECT_ROTATE = Vector( PI / 2, 0, 0 );

enum MDL {
	MDL_NONE,
	MDL_BALL,
	MDL_EARTH,
	MDL_CURSOR,

	MDL_CRYSTAL,
	//MDL_CRYSTAL_1,
	//MDL_CRYSTAL_2,
	//MDL_CRYSTAL_3,
	//MDL_CRYSTAL_4,

	MDL_DELIVERY,

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
	MAX_MDL
};

enum GRAPH {
	GRAPH_TIMER_NUM,
	GRAPH_NUMBER,
	GRAPH_DELIVERY,
	GRAPH_SELECT_MENU,
	GRAPH_GAMEOVER,
	GRAPH_TITLE,
	GRAPH_RESULT,
	GRAPH_MAP,
	GRAPH_STAGE_SELECT_ROGO,
	GRAPH_STAGE_SELECT_1,
	GRAPH_STAGE_SELECT_2,
	GRAPH_STAGE_SELECT_3,
	GRAPH_STAGE_SELECTER,
	GRAPH_PLEASE_PUSH_BUTTON,
	GRAPH_LINK_GAUGE,
	GRAPH_LASER,
	MAX_GRAPH
};

enum EFFECT {
	EFFECT_LASER,
	EFFECT_CATCH_CRYSTAL,
	EFFECT_COL_ROOMBA,
	EFFECT_CRYSTAL_CIRCLE,
};