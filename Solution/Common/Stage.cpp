#include "Stage.h"
#include "Crystal.h"
#include "Application.h"
#include "Binary.h"
#include "Mouse.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Viewer.h"

static const double WALL_POS_Z = wall_size.z * 4;


Stage::Stage( ) {
}

Stage::~Stage( ) {
}

void Stage::drawModel( ) const {
	drawCrystal( );
	drawDelivery( );
}

void Stage::load( int stage_num ) {
	std::string filename = "../Resource/Map/";
	switch ( stage_num ) {
	case 0:
		filename += "00.stage";
		break;
	case 1:
		filename += "01.stage";
		break;
	case 2:
		filename += "02.stage";
		break;
	case 3:
		filename += "test.stage";
		break;
	}

	ApplicationPtr app = Application::getInstance( );
	BinaryPtr binary = BinaryPtr( new Binary );
	if ( !app->loadBinary( filename, binary ) ) {
		return;
	}
	_data = *(DATA*)binary->getPtr( );
}

void Stage::drawCrystal( ) const {
}

void Stage::drawDelivery( ) const {
}

void Stage::loadWall( ) {
	_walls.clear( );
	const int OFFSET_X[ 8 ] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	const int OFFSET_Y[ 8 ] = { -1, -1, 1, 1, -1, 1, 0, 0 };
	const double ROTE[ 4 ] = { PI / 2 * 0, PI / 2 * 1, PI / 2 * 3, PI / 2 * 2 };
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

	for ( int i = 0; i < 16; i++ ) {
		int tmp = 1;
		for ( int j = 0; j < 4; j++ ) {
			tmp *= 2;
			int type = 0;
			if ( i % tmp >= tmp / 2 ) {
				type = 1;
			}
		}
	}
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		//•Ç¶¬
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		Vector pos( x * WORLD_SCALE + WORLD_SCALE / 2, y * WORLD_SCALE + WORLD_SCALE / 2, WALL_POS_Z );
		int type = _data.wall[ i ];
		if ( type != 0 && type != 1 ) {
			continue;
		}
		unsigned char flag = 0;
		for ( int j = 0; j < 4; j++ ) {
			int idx0 = i + OFFSET_X[ j ];
			int idx1 = i + OFFSET_Y[ j ] * STAGE_WIDTH_NUM;
			if ( x + OFFSET_X[ j ] < 0 ) {
				idx0 += STAGE_WIDTH_NUM;
			}
			if ( x + OFFSET_X[ j ] >= STAGE_WIDTH_NUM ) {
				idx0 -= STAGE_WIDTH_NUM;
			}
			if ( y + OFFSET_Y[ j ] < 0 ) {
				idx1 += STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM;
			}
			if ( y + OFFSET_Y[ j ] >= STAGE_HEIGHT_NUM ) {
				idx1 -= STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM;
			}
			if ( type == 1 ) {
				if ( _data.wall[ idx0 ] == 0 && _data.wall[ idx1 ] == 0 ) {
					flag |= 1 << j;
				}
			}
			if ( type == 0 ) {
				if ( _data.wall[ idx0 ] == 1 && _data.wall[ idx1 ] == 1 ) {
					flag |= 1 << j;
				}
			}
		}
		Vector adjust_pos;
		if ( type == 1 ) {
			if ( flag == 1 ||
				 flag == 3 ||
				 flag == 5 ||
				 flag == 15 ) {
				adjust_pos = Vector( -WORLD_SCALE / 2, 0 );
			}
		}
		if ( type == 0 ) {
			adjust_pos += ADJUST[ flag ];
		}
		//adjust_pos -= Vector( WORLD_SCALE, WORLD_SCALE );
		if ( type == 0 && flag == 0 ) {
				continue;
		}
		MDL wall_type = (MDL)( MDL_WALL_0_0 + type * 16 + flag );
		_walls.push_back( Drawer::ModelMDL( pos - adjust_pos, wall_type ) );
	}
}

void Stage::loadPhase( ) {
	_phase++;
	if ( _phase >= MAX_PHASE ) {
		_phase = MAX_PHASE - 1;
		_finished = true;
		return;
	}
	loadCrystal( );
	_max_delivery = 0;
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _data.delivery[ _phase ][ i ] != 0 ) {
			_max_delivery++;
		}
	}
	loadDelivery( );
}

void Stage::loadCrystal( ) {

}

void Stage::loadDelivery( ) {

}

int Stage::getPhase( ) const {
	return _phase;
}

bool Stage::isFinished( ) const {
	return _finished;
}

void Stage::reset( ) {
	_phase = -1;
	_finished = false;
	loadWall( );
	loadPhase( );
}

void Stage::setPhase( int phase ) {
	if ( phase < 0 || phase >= MAX_PHASE ) {
		return;
	}
	_phase = phase;
	loadCrystal( );
}

void Stage::setData( DATA& data ) {
	_data = data;
}

const Stage::DATA& Stage::getData( ) const {
	return _data;
}

void Stage::debug( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	for ( int i = 0; i < MAX_STAGE; i++ ) {
		std::string num = std::to_string( i );
		if ( keyboard->isPushKey( num ) ) {
			load( i );
			reset( );
		}
	}
}

int Stage::getMaxDeliveryNum( ) const {
	return _max_delivery;
}


const std::vector< Drawer::ModelMDL >& Stage::getWalls( ) const {
	return _walls;
}