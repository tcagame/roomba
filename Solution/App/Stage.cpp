#include "Stage.h"
#include "Model.h"
#include "Crystal.h"
#include "define.h"

static const Vector MODEL_SIZE( 6, 6 );
static const double PITCH = 4;

Stage::Stage( ) :
_wave( 0 ),
_finished( false ) {
	_stage_data = { 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
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
			Vector pos = Vector( ( i % STAGE_WIDTH_NUM ) * PITCH + PITCH / 2, ( i / STAGE_WIDTH_NUM ) * PITCH + PITCH / 2, 2 );
			_crystals.push_back( CrystalPtr( new Crystal( pos ) ) );
		}
	}
	_wave++;
}

void Stage::loadModel( ) {
	const int OFFSET_X[ 8 ] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	const int OFFSET_Y[ 8 ] = { -1, -1, 1, 1, -1, 1, 0, 0 };
	const double ROTE[ 4 ] = { PI / 2 * 0, PI / 2 * 1, PI / 2 * 3, PI / 2 * 2 };

	ModelPtr wall_mdl[ 2 ][ 16 ];
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
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		Vector pos( x * PITCH, y * PITCH, -PITCH / 2 );
		// 地面生成
		_earth[ i ] = ModelPtr( new Model );
		_earth[ i ]->mergeModel( earth );
		_earth[ i ]->setPos( pos );
		_earth[ i ]->setTexture( "../Resource/Model/Stage/earth.jpg" );

		//壁生成
		int type = _stage_data[ i ];
		if ( type != 0 && type != 1 ) {
			continue;
		}
		pos += Vector( PITCH / 2, PITCH / 2, PITCH * 2 );
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
		if ( type == 1 ) {
			if ( flag == 1 ||
				 flag == 3 ||
				 flag == 5 ||
				 flag == 15 ) {
				adjust_pos = Vector( -PITCH / 2, 0, 0 );
			}
		}
		if ( type == 0 ) {
			const Vector ADJUST[ 16 ] {
				Vector( 0, 0 ),
				Vector( -PITCH / 8, -PITCH / 2 ),
				Vector( PITCH / 2, -PITCH / 8 ),
				Vector( -PITCH / 8, -PITCH / 2 ),
				Vector( -PITCH / 2, PITCH / 8 ),
				Vector( -PITCH / 8, -PITCH / 2 ),
				Vector( 0, 0 ),
				Vector( 0, 0 ),
				Vector( PITCH / 8, PITCH / 2 ),
				Vector( 0, 0 ),
				Vector( PITCH / 2, -PITCH / 8 ),
				Vector( 0, 0 ),
				Vector( -PITCH / 2, PITCH / 8 ),
				Vector( 0, 0 ),
				Vector( 0, 0 ),
				Vector( -PITCH / 8, -PITCH / 2 )
			};

			adjust_pos += ADJUST[ flag ];
		}
		
		if ( type == 0 && flag == 0 ) {
				continue;
		}
		ModelPtr wall_ptr( new Model );
		wall_ptr->mergeModel( wall_mdl[ type ][ flag ] );
		wall_ptr->setPos( pos + adjust_pos );
		wall_ptr->setTexture( "../Resource/Model/Stage/wall.jpg" );
		_wall.push_back( wall_ptr );
	}
}


bool Stage::isCollisionWall( Vector pos ) {
	// ボールと壁の当たり判定
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _stage_data[ i ] == 1 ) {
			Vector vec1 = Vector( ( i % STAGE_WIDTH_NUM ) * PITCH, ( i / STAGE_WIDTH_NUM ) * PITCH, 0 );
			Vector vec2 = vec1 + Vector( PITCH, 0, 0 );
			Vector vec3 = vec1 + Vector( 0, PITCH, 0 );
			if ( vec1.x < pos.x && pos.x < vec2.x &&
				 vec1.y < pos.y && pos.y < vec3.y ) {
				return true;
			}
		}
	}

	return false;
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

