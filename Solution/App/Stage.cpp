#include "Stage.h"
#include "Model.h"
#include "Crystal.h"
#include "define.h"

static const double MODEL_SIZE = 4;

Stage::Stage( ) :
_wave( 0 ),
_finished( false ) {
	_stage_data = { 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
																															 
	};																														 
	//クリスタル																											    
	_waves[ 0 ] = {																											 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,


	};
	loadCrystalData( );
	loadModel( );
}

Stage::~Stage( ) {
}

void Stage::update( ) {
	updateCrystal( );
}

void Stage::updateCrystal( ) {
	if ( _crystals.size( ) == 0 ) {
		loadCrystalData( );
	}
	std::list< CrystalPtr >::const_iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = (*ite);
		if ( !crystal ) {
			ite++;
			continue;
		}
		if ( crystal->isFinished( ) ) {
			ite = _crystals.erase( ite );
			continue;
		}
		ite++;
	}
}

void Stage::draw( ) {
	drawBackground( );
	drawEarth( );
	drawWall( );
	drawCollisionLine( );
	drawCrystal( );
}

void Stage::drawBackground( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setModelMDL( Drawer::ModelMDL( Vector( ), MDL_BG ) );
}

void Stage::drawEarth( ) const {
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		_earth[ i ]->draw( );
	}
}

void Stage::drawWall( ) const {
	std::vector< ModelPtr >::const_iterator ite = _wall.begin( );
	while ( ite != _wall.end( ) ) {
		if ( !(*ite) ) {
			continue;
		}
		(*ite)->draw( );
		ite++;
	}
}

void Stage::drawCrystal( ) const {
	std::list< CrystalPtr >::const_iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		if ( !(*ite) ) {
			ite++;
			continue;
		}
		(*ite)->draw( );
		ite++;
	}
}

void Stage::loadCrystalData( ) {
	_crystals.clear( );
	if ( _wave >= MAX_WAVE ) {
		_finished = true;
		return;
	}
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _waves[ _wave ][ i ] == 1 ) {
			Vector pos = Vector( ( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, ( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, WORLD_SCALE );
			_crystals.push_back( CrystalPtr( new Crystal( pos ) ) );
		}
	}
	_wave++;
}

void Stage::loadModel( ) {
	const int OFFSET_X[ 8 ] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	const int OFFSET_Y[ 8 ] = { -1, -1, 1, 1, -1, 1, 0, 0 };
	const double ROTE[ 4 ] = { PI / 2 * 0, PI / 2 * 1, PI / 2 * 3, PI / 2 * 2 };
	// あたり判定用データ生成　stage_data拡張
	const int PATTERN[ 16 ][ 4 ] = {
		{ 0, 0, 0, 0 }, // type == 0 の時0、type == 1の時9
		{ 4, 0, 0, 0 },
		{ 0, 3, 0, 0 },
		{ 4, 3, 0, 0 },
		{ 0, 0, 2, 0 },
		{ 4, 0, 2, 0 },
		{ 0, 3, 2, 0 },
		{ 4, 3, 2, 0 },
		{ 0, 0, 0, 1 },
		{ 4, 0, 0, 1 },
		{ 0, 3, 0, 1 },
		{ 4, 3, 0, 1 },
		{ 0, 0, 2, 1 },
		{ 4, 0, 2, 1 },
		{ 0, 3, 2, 1 },
		{ 4, 3, 2, 1 },
	};
	ModelPtr wall_mdl[ 2 ][ 16 ];
	int wall_data[ 2 ][ 16 ][ 4 ] = { 0 };
	for ( int i = 0; i < 2; i++ ) {
		for ( int j = 0; j < 16; j++ ) {
			wall_mdl[ i ][ j ] = ModelPtr( new Model );
		}
	}
	wall_mdl[ 0 ][ 0  ]->load( "../Resource/Model/Stage/0_0.mdl" );
	wall_mdl[ 0 ][ 1  ]->load( "../Resource/Model/Stage/0_1.mdl" );
	wall_mdl[ 0 ][ 2  ]->load( "../Resource/Model/Stage/0_2.mdl" );
	wall_mdl[ 0 ][ 3  ]->load( "../Resource/Model/Stage/0_3.mdl" );
	wall_mdl[ 0 ][ 4  ]->load( "../Resource/Model/Stage/0_4.mdl" );
	wall_mdl[ 0 ][ 5  ]->load( "../Resource/Model/Stage/0_5.mdl" );
	wall_mdl[ 0 ][ 6  ]->load( "../Resource/Model/Stage/0_6.mdl" );
	wall_mdl[ 0 ][ 7  ]->load( "../Resource/Model/Stage/0_7.mdl" );
	wall_mdl[ 0 ][ 8  ]->load( "../Resource/Model/Stage/0_8.mdl" );
	wall_mdl[ 0 ][ 9  ]->load( "../Resource/Model/Stage/0_9.mdl" );
	wall_mdl[ 0 ][ 10 ]->load( "../Resource/Model/Stage/0_10.mdl" );
	wall_mdl[ 0 ][ 11 ]->load( "../Resource/Model/Stage/0_11.mdl" );
	wall_mdl[ 0 ][ 12 ]->load( "../Resource/Model/Stage/0_12.mdl" );
	wall_mdl[ 0 ][ 13 ]->load( "../Resource/Model/Stage/0_13.mdl" );
	wall_mdl[ 0 ][ 14 ]->load( "../Resource/Model/Stage/0_14.mdl" );
	wall_mdl[ 0 ][ 15 ]->load( "../Resource/Model/Stage/0_15.mdl" );

	wall_mdl[ 1 ][ 0  ]->load( "../Resource/Model/Stage/1_0.mdl" );
	wall_mdl[ 1 ][ 1  ]->load( "../Resource/Model/Stage/1_1.mdl" );
	wall_mdl[ 1 ][ 2  ]->load( "../Resource/Model/Stage/1_2.mdl" );
	wall_mdl[ 1 ][ 3  ]->load( "../Resource/Model/Stage/1_3.mdl" );
	wall_mdl[ 1 ][ 4  ]->load( "../Resource/Model/Stage/1_4.mdl" );
	wall_mdl[ 1 ][ 5  ]->load( "../Resource/Model/Stage/1_5.mdl" );
	wall_mdl[ 1 ][ 6  ]->load( "../Resource/Model/Stage/1_6.mdl" );
	wall_mdl[ 1 ][ 7  ]->load( "../Resource/Model/Stage/1_7.mdl" );
	wall_mdl[ 1 ][ 8  ]->load( "../Resource/Model/Stage/1_8.mdl" );
	wall_mdl[ 1 ][ 9  ]->load( "../Resource/Model/Stage/1_9.mdl" );
	wall_mdl[ 1 ][ 10 ]->load( "../Resource/Model/Stage/1_10.mdl" );
	wall_mdl[ 1 ][ 11 ]->load( "../Resource/Model/Stage/1_11.mdl" );
	wall_mdl[ 1 ][ 12 ]->load( "../Resource/Model/Stage/1_12.mdl" );
	wall_mdl[ 1 ][ 13 ]->load( "../Resource/Model/Stage/1_13.mdl" );
	wall_mdl[ 1 ][ 14 ]->load( "../Resource/Model/Stage/1_14.mdl" );
	wall_mdl[ 1 ][ 15 ]->load( "../Resource/Model/Stage/1_15.mdl" );
	/*
	wall_data[ 0 ][ 0  ][ 0 ] = 0;	wall_data[ 0 ][ 0  ][ 1 ] = 0;	wall_data[ 0 ][ 0  ][ 2 ] = 0;	wall_data[ 0 ][ 0  ][ 3 ] = 0;
	wall_data[ 0 ][ 1  ][ 0 ] = 0;	wall_data[ 0 ][ 1  ][ 1 ] = 0;	wall_data[ 0 ][ 1  ][ 2 ] = 0;	wall_data[ 0 ][ 1  ][ 3 ] = 0;
	wall_data[ 0 ][ 2  ][ 0 ] = 0;	wall_data[ 0 ][ 2  ][ 1 ] = 0;	wall_data[ 0 ][ 2  ][ 2 ] = 0;	wall_data[ 0 ][ 2  ][ 3 ] = 0;
	wall_data[ 0 ][ 3  ][ 0 ] = 0;	wall_data[ 0 ][ 3  ][ 1 ] = 0;	wall_data[ 0 ][ 3  ][ 2 ] = 0;	wall_data[ 0 ][ 3  ][ 3 ] = 0;
	wall_data[ 0 ][ 4  ][ 0 ] = 0;	wall_data[ 0 ][ 4  ][ 1 ] = 0;	wall_data[ 0 ][ 4  ][ 2 ] = 0;	wall_data[ 0 ][ 4  ][ 3 ] = 0;
	wall_data[ 0 ][ 5  ][ 0 ] = 0;	wall_data[ 0 ][ 5  ][ 1 ] = 0;	wall_data[ 0 ][ 5  ][ 2 ] = 0;	wall_data[ 0 ][ 5  ][ 3 ] = 0;
	wall_data[ 0 ][ 6  ][ 0 ] = 0;	wall_data[ 0 ][ 6  ][ 1 ] = 0;	wall_data[ 0 ][ 6  ][ 2 ] = 0;	wall_data[ 0 ][ 6  ][ 3 ] = 0;
	wall_data[ 0 ][ 7  ][ 0 ] = 0;	wall_data[ 0 ][ 7  ][ 1 ] = 0;	wall_data[ 0 ][ 7  ][ 2 ] = 0;	wall_data[ 0 ][ 7  ][ 3 ] = 0;
	wall_data[ 0 ][ 8  ][ 0 ] = 0;	wall_data[ 0 ][ 8  ][ 1 ] = 0;	wall_data[ 0 ][ 8  ][ 2 ] = 0;	wall_data[ 0 ][ 8  ][ 3 ] = 0;
	wall_data[ 0 ][ 9  ][ 0 ] = 0;	wall_data[ 0 ][ 9  ][ 1 ] = 0;	wall_data[ 0 ][ 9  ][ 2 ] = 0;	wall_data[ 0 ][ 9  ][ 3 ] = 0;
	wall_data[ 0 ][ 10 ][ 0 ] = 0;	wall_data[ 0 ][ 10 ][ 1 ] = 0;	wall_data[ 0 ][ 10 ][ 2 ] = 0;	wall_data[ 0 ][ 10 ][ 3 ] = 0;
	wall_data[ 0 ][ 11 ][ 0 ] = 0;	wall_data[ 0 ][ 11 ][ 1 ] = 0;	wall_data[ 0 ][ 11 ][ 2 ] = 0;	wall_data[ 0 ][ 11 ][ 3 ] = 0;
	wall_data[ 0 ][ 12 ][ 0 ] = 0;	wall_data[ 0 ][ 12 ][ 1 ] = 0;	wall_data[ 0 ][ 12 ][ 2 ] = 0;	wall_data[ 0 ][ 12 ][ 3 ] = 0;
	wall_data[ 0 ][ 13 ][ 0 ] = 0;	wall_data[ 0 ][ 13 ][ 1 ] = 0;	wall_data[ 0 ][ 13 ][ 2 ] = 0;	wall_data[ 0 ][ 13 ][ 3 ] = 0;
	wall_data[ 0 ][ 14 ][ 0 ] = 0;	wall_data[ 0 ][ 14 ][ 1 ] = 0;	wall_data[ 0 ][ 14 ][ 2 ] = 0;	wall_data[ 0 ][ 14 ][ 3 ] = 0;
	wall_data[ 0 ][ 15 ][ 0 ] = 0;	wall_data[ 0 ][ 15 ][ 1 ] = 0;	wall_data[ 0 ][ 15 ][ 2 ] = 0;	wall_data[ 0 ][ 15 ][ 3 ] = 0;

	wall_data[ 1 ][ 0  ][ 0 ] = 0;	wall_data[ 1 ][ 0  ][ 1 ] = 0;	wall_data[ 1 ][ 0  ][ 2 ] = 0;	wall_data[ 1 ][ 0  ][ 3 ] = 0;
	wall_data[ 1 ][ 1  ][ 0 ] = 0;	wall_data[ 1 ][ 1  ][ 1 ] = 0;	wall_data[ 1 ][ 1  ][ 2 ] = 0;	wall_data[ 1 ][ 1  ][ 3 ] = 0;
	wall_data[ 1 ][ 2  ][ 0 ] = 0;	wall_data[ 1 ][ 2  ][ 1 ] = 0;	wall_data[ 1 ][ 2  ][ 2 ] = 0;	wall_data[ 1 ][ 2  ][ 3 ] = 0;
	wall_data[ 1 ][ 3  ][ 0 ] = 0;	wall_data[ 1 ][ 3  ][ 1 ] = 0;	wall_data[ 1 ][ 3  ][ 2 ] = 0;	wall_data[ 1 ][ 3  ][ 3 ] = 0;
	wall_data[ 1 ][ 4  ][ 0 ] = 0;	wall_data[ 1 ][ 4  ][ 1 ] = 0;	wall_data[ 1 ][ 4  ][ 2 ] = 0;	wall_data[ 1 ][ 4  ][ 3 ] = 0;
	wall_data[ 1 ][ 5  ][ 0 ] = 0;	wall_data[ 1 ][ 5  ][ 1 ] = 0;	wall_data[ 1 ][ 5  ][ 2 ] = 0;	wall_data[ 1 ][ 5  ][ 3 ] = 0;
	wall_data[ 1 ][ 6  ][ 0 ] = 0;	wall_data[ 1 ][ 6  ][ 1 ] = 0;	wall_data[ 1 ][ 6  ][ 2 ] = 0;	wall_data[ 1 ][ 6  ][ 3 ] = 0;
	wall_data[ 1 ][ 7  ][ 0 ] = 0;	wall_data[ 1 ][ 7  ][ 1 ] = 0;	wall_data[ 1 ][ 7  ][ 2 ] = 0;	wall_data[ 1 ][ 7  ][ 3 ] = 0;
	wall_data[ 1 ][ 8  ][ 0 ] = 0;	wall_data[ 1 ][ 8  ][ 1 ] = 0;	wall_data[ 1 ][ 8  ][ 2 ] = 0;	wall_data[ 1 ][ 8  ][ 3 ] = 0;
	wall_data[ 1 ][ 9  ][ 0 ] = 0;	wall_data[ 1 ][ 9  ][ 1 ] = 0;	wall_data[ 1 ][ 9  ][ 2 ] = 0;	wall_data[ 1 ][ 9  ][ 3 ] = 0;
	wall_data[ 1 ][ 10 ][ 0 ] = 0;	wall_data[ 1 ][ 10 ][ 1 ] = 0;	wall_data[ 1 ][ 10 ][ 2 ] = 0;	wall_data[ 1 ][ 10 ][ 3 ] = 0;
	wall_data[ 1 ][ 11 ][ 0 ] = 0;	wall_data[ 1 ][ 11 ][ 1 ] = 0;	wall_data[ 1 ][ 11 ][ 2 ] = 0;	wall_data[ 1 ][ 11 ][ 3 ] = 0;
	wall_data[ 1 ][ 12 ][ 0 ] = 0;	wall_data[ 1 ][ 12 ][ 1 ] = 0;	wall_data[ 1 ][ 12 ][ 2 ] = 0;	wall_data[ 1 ][ 12 ][ 3 ] = 0;
	wall_data[ 1 ][ 13 ][ 0 ] = 0;	wall_data[ 1 ][ 13 ][ 1 ] = 0;	wall_data[ 1 ][ 13 ][ 2 ] = 0;	wall_data[ 1 ][ 13 ][ 3 ] = 0;
	wall_data[ 1 ][ 14 ][ 0 ] = 0;	wall_data[ 1 ][ 14 ][ 1 ] = 0;	wall_data[ 1 ][ 14 ][ 2 ] = 0;	wall_data[ 1 ][ 14 ][ 3 ] = 0;
	wall_data[ 1 ][ 15 ][ 0 ] = 0;	wall_data[ 1 ][ 15 ][ 1 ] = 0;	wall_data[ 1 ][ 15 ][ 2 ] = 0;	wall_data[ 1 ][ 15 ][ 3 ] = 0;

	for ( int i = 0; i < 2 * 16; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			wall_data[ i / 16 ][ i % 16 ][ j ] = 0;
		}
	}
	*/
/*	
	ModelPtr model = ModelPtr( new Model );
	model->load( "../Resource/Model/Stage/wall_2.mdl" );
	ModelPtr aaa[ 16 ];
	for ( int i = 0; i < 16; i++ ) {
		aaa[ i ] = ModelPtr( new Model );
		int tmp = 1;
		for ( int j = 0; j < 4; j++ ) {
			tmp *= 2;
			if ( i % tmp >= tmp / 2 ) {
				ModelPtr tmp_model( new Model );
				tmp_model->mergeModel( model );
				tmp_model->multiply( Matrix::makeTransformRotation( Vector( 0, 0, -1 ), ROTE[ j ] ) );
				aaa[ i ]->mergeModel( tmp_model );
			}
		}
	}
	aaa[ 0  ]->save( "../Resource/Model/Stage/0_0.mdl" );
	aaa[ 1  ]->save( "../Resource/Model/Stage/0_1.mdl" );
	aaa[ 2  ]->save( "../Resource/Model/Stage/0_2.mdl" );
	aaa[ 3  ]->save( "../Resource/Model/Stage/0_3.mdl" );
	aaa[ 4  ]->save( "../Resource/Model/Stage/0_4.mdl" );
	aaa[ 5  ]->save( "../Resource/Model/Stage/0_5.mdl" );
	aaa[ 6  ]->save( "../Resource/Model/Stage/0_6.mdl" );
	aaa[ 7  ]->save( "../Resource/Model/Stage/0_7.mdl" );
	aaa[ 8  ]->save( "../Resource/Model/Stage/0_8.mdl" );
	aaa[ 9  ]->save( "../Resource/Model/Stage/0_9.mdl" );
	aaa[ 10 ]->save( "../Resource/Model/Stage/0_10.mdl" );
	aaa[ 11 ]->save( "../Resource/Model/Stage/0_11.mdl" );
	aaa[ 12 ]->save( "../Resource/Model/Stage/0_12.mdl" );
	aaa[ 13 ]->save( "../Resource/Model/Stage/0_13.mdl" );
	aaa[ 14 ]->save( "../Resource/Model/Stage/0_14.mdl" );
	aaa[ 15 ]->save( "../Resource/Model/Stage/0_15.mdl" );
*/
	ModelPtr earth( new Model );
	earth->load( "../Resource/Model/Stage/earth.mdl" );
	Matrix size = Matrix::makeTransformScaling( Vector( WORLD_SCALE / MODEL_SIZE, WORLD_SCALE / MODEL_SIZE, WORLD_SCALE / MODEL_SIZE ) );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		Vector pos( x * WORLD_SCALE, y * WORLD_SCALE, 0 );
		// 地面生成
		_earth[ i ] = ModelPtr( new Model );
		_earth[ i ]->mergeModel( earth );
		_earth[ i ]->multiply( size );
		_earth[ i ]->setPos( pos );
		_earth[ i ]->setTexture( "../Resource/Model/Stage/earth.jpg" );

		//壁生成
		int type = _stage_data[ i ];
		if ( type != 0 && type != 1 ) {
			continue;
		}
		pos += Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, WORLD_SCALE * 2 );
		unsigned char flag = 0;
		Vector adjust_pos = Vector( );
		for ( int j = 0; j < 4; j++ ) {
			if ( i % STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM < OFFSET_X[ j ] ||
				 i % STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM  + OFFSET_X[ j ] > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
				continue;
			}
			int idx0 = i + OFFSET_X[ j ];
			if ( idx0 < 0 || idx0 >= STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
				continue;
			}
	
			int idx1 = i + OFFSET_Y[ j ] * STAGE_WIDTH_NUM;
			if ( idx1 < 0 || idx1 >= STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
				continue;
			}
			if ( type == 1 ) {
				flag |= ( _stage_data[ idx0 ] == 0 && _stage_data[ idx1 ] == 0 ) << j;
			}
			if ( type == 0 ) {
				flag |= ( _stage_data[ idx0 ] == 1 && _stage_data[ idx1 ] == 1 ) << j;
			}
		}
		// あたり判定用データ生成　stage_data拡張
#if 1
		for ( int j = 0; j < 4; j++ ) {
			int idx = ( x * 2 + j % 2 ) + ( y * 2 * STAGE_WIDTH_NUM * 2 + j / 2 * STAGE_WIDTH_NUM * 2 );
			if ( _stage_data[ i ] ) {
				_map_data[ idx ] = 1;
			} else {
				_map_data[ idx ] = 0;
			}
		}
#endif
		if ( type == 1 ) {
			if ( flag == 1 ||
				 flag == 3 ||
				 flag == 5 ||
				 flag == 15 ) {
				adjust_pos = Vector( -WORLD_SCALE / 2, 0, 0 );
			}
		}
		if ( type == 0 ) {
			const Vector ADJUST[ 16 ] {
				Vector( 0, 0 ),
				Vector( -WORLD_SCALE / 8, -WORLD_SCALE / 2 ),
				Vector( WORLD_SCALE / 2, -WORLD_SCALE / 8 ),
				Vector( -WORLD_SCALE / 8, -WORLD_SCALE / 2 ),
				Vector( -WORLD_SCALE / 2, WORLD_SCALE / 8 ),
				Vector( -WORLD_SCALE / 8, -WORLD_SCALE / 2 ),
				Vector( 0, 0 ),
				Vector( 0, 0 ),
				Vector( WORLD_SCALE / 8, WORLD_SCALE / 2 ),
				Vector( 0, 0 ),
				Vector( WORLD_SCALE / 2, -WORLD_SCALE / 8 ),
				Vector( 0, 0 ),
				Vector( -WORLD_SCALE / 2, WORLD_SCALE / 8 ),
				Vector( 0, 0 ),
				Vector( 0, 0 ),
				Vector( -WORLD_SCALE / 8, -WORLD_SCALE / 2 )
			};

			adjust_pos += ADJUST[ flag ];
		}
		
		if ( type == 0 && flag == 0 ) {
				continue;
		}
		ModelPtr wall_ptr( new Model );
		wall_ptr->mergeModel( wall_mdl[ type ][ flag ] );
		wall_ptr->multiply( size );
		wall_ptr->setPos( pos + adjust_pos );
		wall_ptr->setTexture( "../Resource/Model/Stage/wall.jpg" );
		_wall.push_back( wall_ptr );
	}
}

void Stage::drawCollisionLine( ) const {
    DrawerPtr drawer = Drawer::getTask( );
	
	for ( int i = 0; i <= STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2; i++ ) {
		int x = i % ( STAGE_WIDTH_NUM * 2 );
		int y = i / ( STAGE_WIDTH_NUM * 2 );
		drawer->drawLine( Vector( 0, y * WORLD_SCALE / 2, 2.2 ), Vector( STAGE_WIDTH_NUM * WORLD_SCALE, y * WORLD_SCALE / 2, 2.2 ) );
		drawer->drawLine( Vector( x * WORLD_SCALE / 2, 0, 2.2 ), Vector( x * WORLD_SCALE / 2, STAGE_HEIGHT_NUM * WORLD_SCALE, 2.2 ) );
	}

    for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
        if ( _stage_data[ i ] == 1 ) {
            Vector vec1 = Vector( ( i % STAGE_WIDTH_NUM ) * WORLD_SCALE, ( i / STAGE_WIDTH_NUM ) * WORLD_SCALE, WORLD_SCALE * 2 + 1 );
            Vector vec2 = vec1 + Vector( WORLD_SCALE, 0, 0 );
            Vector vec3 = vec1 + Vector( 0, WORLD_SCALE, 0 );
            Vector vec4 = vec1 + Vector( WORLD_SCALE, WORLD_SCALE, 0 );

            Vector vec5 = vec1 + Vector( WORLD_SCALE / 2, 0, 0 );
            Vector vec6 = vec1 + Vector( WORLD_SCALE, WORLD_SCALE / 2, 0 );
            Vector vec7 = vec1 + Vector( 0, WORLD_SCALE / 2, 0 );
            Vector vec8 = vec1 + Vector( WORLD_SCALE / 2, WORLD_SCALE, 0 );
            Vector vec9 = vec1 + Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, 0 );
			// 縦棒
            drawer->drawLine( vec1 - Vector( 0, 0, WORLD_SCALE * 2 ), vec1 );
            drawer->drawLine( vec2 - Vector( 0, 0, WORLD_SCALE * 2 ), vec2 );
            drawer->drawLine( vec3 - Vector( 0, 0, WORLD_SCALE * 2 ), vec3 );
            drawer->drawLine( vec4 - Vector( 0, 0, WORLD_SCALE * 2 ), vec4 );
            drawer->drawLine( vec5 - Vector( 0, 0, WORLD_SCALE * 2 ), vec5 );
            drawer->drawLine( vec6 - Vector( 0, 0, WORLD_SCALE * 2 ), vec6 );
            drawer->drawLine( vec7 - Vector( 0, 0, WORLD_SCALE * 2 ), vec7 );
            drawer->drawLine( vec8 - Vector( 0, 0, WORLD_SCALE * 2 ), vec8 );
            drawer->drawLine( vec9 - Vector( 0, 0, WORLD_SCALE * 2 ), vec9 );
			//　天井の横棒
            drawer->drawLine( vec1, vec2 );
            drawer->drawLine( vec2, vec4 );
            drawer->drawLine( vec3, vec4 );
            drawer->drawLine( vec3, vec1 );
        }
    }
}


Vector Stage::getCollisionWall( Vector pos, Vector vec, const double radius ) {
	// ボールと壁の当たり判定
	Vector result = vec;
	int f_pos_x = (int)( ( pos.x + vec.x + ( Vector( vec.x, 0, 0 ).normalize( ) * radius ).x ) );
	int f_pos_y = (int)( ( pos.y + vec.y + ( Vector( 0, vec.y, 0 ).normalize( ) * radius ).y ) );

	int idx_x = f_pos_x - 0.5;
	int idx_y = f_pos_y - 0.5;
	int idx = idx_x + idx_y * STAGE_WIDTH_NUM * 2;

	if ( _map_data[ idx ] == 1 ) {
		Vector vec1 = Vector( ( idx_x % STAGE_WIDTH_NUM ) * WORLD_SCALE, ( idx_y / STAGE_WIDTH_NUM ) * WORLD_SCALE, 0 );
		Vector vec2 = vec1 + Vector( WORLD_SCALE, 0, 0 );
		Vector vec3 = vec1 + Vector( 0, WORLD_SCALE, 0 );

		if ( vec1.x < f_pos_x && f_pos_x < vec2.x ) {
			result.x = 0;
		}
		if ( vec1.y < f_pos_y && f_pos_y < vec3.y ) {
			result.y = 0;
		}
	}

	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 10, 10, "x:%d  y:%d  idx:%d", f_pos_x, f_pos_y, idx );

	return result;
}

bool Stage::isFinished( ) const {
	return _finished;
}

CrystalPtr Stage::getHittingCrystal( Vector pos0, Vector pos1 ) {
	CrystalPtr hitting = CrystalPtr( );
	//あたっているクリスタルをhittingに代入する
	std::list< CrystalPtr >::const_iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = (*ite);
		if ( !crystal ) {
			ite++;
			continue;
		}
		if ( crystal->isHitting( pos0, pos1 ) ) {
			hitting = crystal;
		}
		ite++;
	}
	return hitting;
}

void Stage::reset( ) {
	_wave = 0;
	_finished = false;
	loadCrystalData( );
}

