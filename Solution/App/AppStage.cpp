#include "AppStage.h"
#include "Application.h"
#include "Binary.h"
#include "define.h"
#include "Crystal.h"

AppStage::AppStage( int stage_num ) :
Stage( ) {
	load( stage_num );//0~2:通常 3:test_stage
	reset( );
}


AppStage::~AppStage( ) {
}

void AppStage::reset( ) {
	loadMapData( );
	Stage::reset( );
}

void AppStage::update( ) {
	updateCrystal( );
	debug( );
}

void AppStage::draw( ) const {
	drawModel( );
}

void AppStage::load( int stage_num ) {
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
	setData( *(DATA*)binary->getPtr( ) );
}

void AppStage::updateCrystal( ) {
	if ( _crystals.size( ) == 0 ) {
		loadPhase( );
	}
	ApplicationPtr app = Application::getInstance( );
	int scr_width = app->getWindowWidth( );
	DrawerPtr drawer = Drawer::getTask( );
	std::list< CrystalPtr >::iterator ite = _crystals.begin( );
	int num = 0;
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = (*ite);
		if ( !crystal ) {
			ite++;
			continue;
		}
		if ( crystal->isFinished( ) ) {
			crystal.~shared_ptr( );
			ite = _crystals.erase( ite );
			continue;
		}
		AppStagePtr stage = std::dynamic_pointer_cast< AppStage >( shared_from_this( ) );
		crystal->update( stage );
		Vector pos = crystal->getPos( );
		drawer->drawString( scr_width - 280, num * 20, "[クリスタル%2d] x:%04.1f y:%04.1f", num, pos.x, pos.y );
		num++;
		ite++;
	}
}


void AppStage::loadCrystal( ) {
	std::list< CrystalPtr >::const_iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = *ite;
		if ( !crystal ) {
			ite++;
			continue;
		}
		crystal.~shared_ptr( );
		ite++;
	}
	_crystals.clear( );
	DATA data = getData( );
	int phase = getPhase( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if (  data.crystal[ phase ][ i ] != 0 ) {
			Vector pos = Vector( ( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, ( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, -WORLD_SCALE );
			MDL mdl = (MDL)( (int)MDL_CRYSTAL_0 + data.crystal[ phase ][ i ] - 1 );
			_crystals.push_back( CrystalPtr( new Crystal( pos, mdl ) ) );
		}
	}
}

void AppStage::drawCrystal( ) const {
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

CrystalPtr AppStage::getHittingCrystal( Vector pos0, Vector pos1 ) const {
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

Vector AppStage::adjustCollisionToWall( Vector pos, Vector vec, const double radius ) {
	// ボールと壁の当たり判定
	Vector result = vec;
	const int OFFSET_X[ 8 ] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	const int OFFSET_Y[ 8 ] = { -1, -1, 1, 1, -1, 1, 0, 0 };
	const Vector DIR[ 4 ] = {
		Vector( -1, 1 ),  // 左上 
		Vector( 1, 1 ),	  // 右上
		Vector( -1, -1 ), // 左下
		Vector( 1, -1 )	  // 右下
	};

	double x = pos.x + vec.x;
	double y = pos.y + vec.y;
	for ( int i = 0; i < 2; i++ ) {
		double rad = radius;
		if ( i == 0 ) {
			rad *= -1;
		}
		double fy = pos.y + vec.y + rad;
		double fx = pos.x + vec.x + rad;
		int idx_x = (int)fx + (int)y * STAGE_WIDTH_NUM * 2;
		int idx_y = (int)x + (int)fy * STAGE_WIDTH_NUM * 2;
		Vector dir = vec.normalize( ) * radius;
		int tmp_x = (int)( pos.x + vec.x + dir.x );
		int tmp_y = (int)( pos.y + vec.y + dir.y );
		int idx = tmp_x + tmp_y * STAGE_WIDTH_NUM * 2;
		if ( idx_x < 0 || idx_x >= STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2 ) {
				return Vector( );
		}
		if ( idx_y < 0 || idx_y >= STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2 ) {
				return Vector( );
		}
		if ( idx < 0 || idx >= STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2 ) {
				return Vector( );
		}
		switch ( _map_data[ idx_x ] ) {
		case 1:
			result.x = vec.x * -1;
			break;
		case 2:
			{
				int add_x = 0;
				int add_y = 0;
				if ( (int)fx % 2 == 0 ) add_x = (int)( WORLD_SCALE / 2 );
				if ( (int)y % 2 == 0 ) add_y = (int)( WORLD_SCALE / 2 );
				Vector check_pos( fx + add_x, y + add_y );
				double length = ( check_pos - Vector( x, y ) ).getLength( );
				if ( length < WORLD_SCALE / 2 + radius ) {
					result.x = vec.x * -1;
				}
			}
			break;
		}
		switch ( _map_data[ idx_y ] ) {
		case 1:
			result.y = vec.y * -1;
			break;
		case 2:
			{
				int add_x = 0;
				int add_y = 0;
				if ( (int)x % 2 == 0 ) add_x = (int)( WORLD_SCALE / 2 );
				if ( (int)fy % 2 == 0 ) add_y = (int)( WORLD_SCALE / 2 );
				Vector check_pos( x + add_x, fy + add_y );
				double length = ( check_pos - Vector( x, y ) ).getLength( );
				if ( length < WORLD_SCALE / 2 + radius ) {
					result.y = vec.y * -1;
				}
			}
			break;
		}
		switch ( _map_data[ idx ] ) {
		case 1:
			break;
		case 2:
			{
				int add_x = 0;
				int add_y = 0;
				if ( tmp_x % 2 == 0 ) add_x = (int)( WORLD_SCALE / 2 );
				if ( tmp_y % 2 == 0 ) add_y = (int)( WORLD_SCALE / 2 );
				Vector check_pos( tmp_x + add_x, tmp_y + add_y );
				double length = ( check_pos - Vector( x, y ) ).getLength( );
				if ( length < WORLD_SCALE / 2 + radius ) {
					result = vec * -1;
				}
			}
			break;
		}
	}
	return result;
}

Vector AppStage::adjustCollisionToCrystal( Vector pos, Vector vec, const double radius ) {
	Vector result;
	std::list< CrystalPtr >::const_iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = (*ite);
		if ( !crystal ) {
			ite++;
			continue;
		}
		Vector adjust = crystal->adjustHitToRoomba( pos, vec, radius );
		result += adjust;
		ite++;
	}
	return result;
}

void AppStage::drawCollisionLine( ) const {
	int phase = getPhase( );
	DATA data = getData( );
	if ( phase >= MAX_PHASE ) {
		return;	
	}
    DrawerPtr drawer = Drawer::getTask( );
	
	for ( int i = 0; i <= STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2; i++ ) {
		int x = i % ( STAGE_WIDTH_NUM * 2 );
		int y = i / ( STAGE_WIDTH_NUM * 2 );
		drawer->drawLine( Vector( 0, y * WORLD_SCALE / 2, -WORLD_SCALE / 2 ), Vector( STAGE_WIDTH_NUM * WORLD_SCALE, y * WORLD_SCALE / 2, -WORLD_SCALE / 2 ) );
		drawer->drawLine( Vector( x * WORLD_SCALE / 2, 0, -WORLD_SCALE / 2 ), Vector( x * WORLD_SCALE / 2, STAGE_HEIGHT_NUM * WORLD_SCALE, -WORLD_SCALE / 2) );
	}

    for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
        if ( data.wall[ i ] == 1 ) {
            Vector vec1 = Vector( ( i % STAGE_WIDTH_NUM ) * WORLD_SCALE, ( i / STAGE_WIDTH_NUM ) * WORLD_SCALE, 0.1 );
            Vector vec2 = vec1 + Vector( WORLD_SCALE, 0, 0 );
            Vector vec3 = vec1 + Vector( 0, WORLD_SCALE, 0 );
            Vector vec4 = vec1 + Vector( WORLD_SCALE, WORLD_SCALE, 0 );

            Vector vec5 = vec1 + Vector( WORLD_SCALE / 2, 0, 0 );
            Vector vec6 = vec1 + Vector( WORLD_SCALE, WORLD_SCALE / 2, 0 );
            Vector vec7 = vec1 + Vector( 0, WORLD_SCALE / 2, 0 );
            Vector vec8 = vec1 + Vector( WORLD_SCALE / 2, WORLD_SCALE, 0 );
            Vector vec9 = vec1 + Vector( WORLD_SCALE / 2, WORLD_SCALE / 2, 0 );
			// 縦棒
            drawer->drawLine( vec1 - Vector( 0, 0, WORLD_SCALE ), vec1 );
            drawer->drawLine( vec2 - Vector( 0, 0, WORLD_SCALE ), vec2 );
            drawer->drawLine( vec3 - Vector( 0, 0, WORLD_SCALE ), vec3 );
            drawer->drawLine( vec4 - Vector( 0, 0, WORLD_SCALE ), vec4 );
            drawer->drawLine( vec5 - Vector( 0, 0, WORLD_SCALE ), vec5 );
            drawer->drawLine( vec6 - Vector( 0, 0, WORLD_SCALE ), vec6 );
            drawer->drawLine( vec7 - Vector( 0, 0, WORLD_SCALE ), vec7 );
            drawer->drawLine( vec8 - Vector( 0, 0, WORLD_SCALE ), vec8 );
            drawer->drawLine( vec9 - Vector( 0, 0, WORLD_SCALE ), vec9 );
			//　天井の横棒
            drawer->drawLine( vec1, vec2 );
            drawer->drawLine( vec2, vec4 );
            drawer->drawLine( vec3, vec4 );
            drawer->drawLine( vec3, vec1 );
        }
    }
}

void AppStage::loadMapData( ) {
	const int OFFSET_X[ 8 ] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	const int OFFSET_Y[ 8 ] = { -1, -1, 1, 1, -1, 1, 0, 0 };

	for ( int i = 0; i < STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2; i++ ) {
		_map_data[ i ] = 0;
	}

	DATA data = getData( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		int type = data.wall[ i ];
		if ( type != 0 && type != 1 ) {
			continue;
		}
		for ( int j = 0; j < 4; j++ ) {
			int tmp_x = x * 2 + j % 2;
			int tmp_y = y * 2 + j / 2;
			int map_idx = tmp_x % ( STAGE_WIDTH_NUM * 2 ) + tmp_y * STAGE_WIDTH_NUM * 2;
			if ( x + OFFSET_X[ j ] < 0 ||
				 x + OFFSET_X[ j ] >= STAGE_WIDTH_NUM ) {
				_map_data[ map_idx ] = 1;
				continue;
			}
			if ( y + OFFSET_Y[ j ] < 0 ||
				 y + OFFSET_Y[ j ] >= STAGE_HEIGHT_NUM ) {
				_map_data[ map_idx ] = 1;
				continue;
			}

			int idx0 = i + OFFSET_X[ j ];
			int idx1 = i + OFFSET_Y[ j ] * STAGE_WIDTH_NUM;

			if ( type == 1 ) {
				_map_data[ map_idx ] = 1;
				if ( data.wall[ idx0 ] == 0 && data.wall[ idx1 ] == 0 ) {
					_map_data[ map_idx ] = 2;
				}
			}
			if ( type == 0 ) {
				if ( data.wall[ idx0 ] == 1 && data.wall[ idx1 ] == 1 ) {
					_map_data[ map_idx ] = 3;
				}
			}
		}
	}
}

bool AppStage::isOnStation( Vector pos, int type ) {
	int x = ( int )( pos.x / WORLD_SCALE );
	int y = ( int )( pos.y / WORLD_SCALE );
	int idx = x + y * STAGE_WIDTH_NUM;

	DATA data = getData( );
	int phase = getPhase( );
	if ( data.station[ phase ][ idx ] == type ) {
		data.station[ phase ][ idx ] = 0;
		setData( data );
		return true;
	}
	return false;
}

int AppStage::getStationNum( ) const {
	int num = 0;
	DATA data = getData( );
	int phase = getPhase( );
	for ( int i = 0; i < STAGE_HEIGHT_NUM * STAGE_WIDTH_NUM; i++ ) {
		if ( data.station[ phase ][ i ] != 0 ) {
			num++;
		}
	}
	return num;
}

std::list< CrystalPtr > AppStage::getCrystalList( ) const {
	return _crystals;
}