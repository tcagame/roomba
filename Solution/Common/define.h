#pragma once

static const double WORLD_SCALE = 2;
static const double STAGE_MODEL_SIZE = 4;
static const int MAX_PHASE = 10;
static const int STAGE_WIDTH_NUM = 40;
static const int STAGE_HEIGHT_NUM = 35;

enum MDL {
	MDL_NONE,
	MDL_BG,
	MDL_BALL,
	MDL_EARTH,
	MDL_CURSOR,

	MDL_CRYSTAL_0,
	MDL_CRYSTAL_1,
	MDL_CRYSTAL_2,
	MDL_CRYSTAL_3,
	MDL_CRYSTAL_4,

	MDL_STATION_0,
	MDL_STATION_1,
	MDL_STATION_2,
	MDL_STATION_3,
	MDL_STATION_4,

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
	GRAPH_STATION,
	GRAPH_SELECT_MENU,
	GRAPH_GAMEOVER,
	GRAPH_TITLE,
	GRAPH_RESULT,
	GRAPH_PHASE,
	GRAPH_MAP,
	STAGE_SELECT_ROGO,
	STAGE_SELECT_1,
	STAGE_SELECT_2,
	STAGE_SELECT_3,
	STAGE_SELECTER,
	MAX_GRAPH
};