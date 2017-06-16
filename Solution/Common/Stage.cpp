#include "Stage.h"
#include "Crystal.h"
#include "Application.h"
#include "Binary.h"
#include "Mouse.h"
#include "Camera.h"
#include "Keyboard.h"

static const double WALL_POS_Z = 0;


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
	loadData( filename );
	_max_delivery = 0;
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _data[ i ].delivery > 0 ) {
			_max_delivery++;
		}
	}

	loadCrystal( );
	loadWall( );
	loadDelivery( );
}

void Stage::loadData( std::string filename ) {
	ApplicationPtr app = Application::getInstance( );
	BinaryPtr binary( new Binary );
	if ( filename.find( ".stage" ) == std::string::npos ) {
		filename += ".stage";
	}
	if ( !app->loadBinary( filename, binary ) ) {
		return;
	}
	_data = *(std::array< DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >(*))binary->getPtr( );
}

void Stage::saveData( std::string filename ) const {
	BinaryPtr binary = BinaryPtr( new Binary );
	binary->ensure( sizeof( _data ) );
	binary->write( (void*)&_data, sizeof( _data ) );
	ApplicationPtr app = Application::getInstance( );
	if ( filename.find( ".stage" ) == std::string::npos ) {
		filename += ".stage";
	}
	app->saveBinary( filename, binary );
}

void Stage::drawCrystal( ) const {
}

void Stage::drawDelivery( ) const {
}

void Stage::loadWall( ) {
	_walls.clear( );
	const int OFFSET_X[ 8 ] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	const int OFFSET_Y[ 8 ] = { -1, -1, 1, 1, -1, 1, 0, 0 };

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
		//�ǐ���
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		Vector pos( x * WORLD_SCALE + WORLD_SCALE / 2, y * WORLD_SCALE + WORLD_SCALE / 2, WALL_POS_Z );
		int type = _data[ i ].wall;
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
				if ( _data[ idx0 ].wall == 0 && _data[ idx1 ].wall == 0 ) {
					flag |= 1 << j;
				}
			}
			if ( type == 0 ) {
				if ( _data[ idx0 ].wall == 1 && _data[ idx1 ].wall == 1 ) {
					flag |= 1 << j;
				}
			}
		}
		if ( type == 0 && flag == 0 ) {
				continue;
		}
		MV1 wall_type = (MV1)( MV1_WALL_0_0 + type * 16 + flag );
		if ( wall_type == MV1_WALL_0_0 ) {
			continue;
		}
		MV1_INFO mv1;
		mv1.pos = pos;
		mv1.type = wall_type;
		_walls.push_back( mv1 );
	}
}

void Stage::loadCrystal( ) {

}

void Stage::loadDelivery( ) {

}

void Stage::reset( ) {
	loadWall( );
}

const Stage::DATA& Stage::getData( int idx ) const {
	return _data[ idx ];
}

const std::array< Stage::DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >& Stage::getData( ) const {
	return _data;
}

void Stage::setData( DATA& data, int idx ) {
	_data[ idx ] = data;
}

void Stage::setData( std::array< DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM >& data ) {
	_data = data;
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


const std::vector< Stage::MV1_INFO >& Stage::getWalls( ) const {
	return _walls;
}