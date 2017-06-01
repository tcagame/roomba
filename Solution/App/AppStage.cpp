#include "AppStage.h"
#include "Application.h"
#include "Binary.h"
#include "define.h"
#include "Crystal.h"
#include "Viewer.h"
#include "Keyboard.h"

AppStage::AppStage( int stage_num, ViewerPtr viewer ) :
_station_count( 1 ),
_viewer( viewer ) {
	load( 3 );//0~2:通常 3:test_stage
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
	KeyboardPtr keyboard = Keyboard::getTask( );
	for ( int i = 0; i < MAX_PHASE; i++ ) {
		std::string num = std::to_string( i );
		if ( keyboard->isPushKey( num ) && keyboard->isHoldKey( "SPACE" ) ) {
			setPhase( i );
		}
	}
}

void AppStage::draw( ) const {
	drawModel( );
	drawEarth( );
	drawWall( );
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
		//Vector pos = crystal->getPos( );
		//drawer->drawString( scr_width - 280, num * 20, "[クリスタル%2d] x:%04.1f y:%04.1f", num, pos.x, pos.y );
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
			pos += Vector( STAGE_WIDTH_NUM * WORLD_SCALE, STAGE_HEIGHT_NUM * WORLD_SCALE );
			_crystals.push_back( CrystalPtr( new Crystal( pos, MDL_CRYSTAL ) ) );
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
		(*ite)->draw( _viewer );
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
	const int OFFSET_X[ 9 ] = { -1, 1, -1, 1, 0, 0, -1, 1, 0 };
	const int OFFSET_Y[ 9 ] = { -1, -1, 1, 1, -1, 1, 0, 0, 0 };
	while ( pos.x > STAGE_WIDTH_NUM * WORLD_SCALE ) {
		pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE ) {	
		pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	Vector fpos = pos + vec;//未来座標
	int x = (int)( fpos.x + 0.5 );
	int y = (int)( fpos.y + 0.5 );
	//未来座標(idx)の周りの状態を知る
	for ( int i = 0; i < 9; i++ ) {
		int fx = x + OFFSET_X[ i ];
		int fy = y + OFFSET_Y[ i ];
		if ( fx >= STAGE_WIDTH_NUM * 2 ) {
			fx -= STAGE_WIDTH_NUM * 2;
		}
		if ( fx < 0 ) {
			fx += STAGE_WIDTH_NUM * 2;
		}
		if ( fy >= STAGE_HEIGHT_NUM * 2 ) {
			fy -= STAGE_HEIGHT_NUM * 2;
		}
		if ( fy < 0 ) {
			fy += STAGE_HEIGHT_NUM * 2;
		}
		int idx = fx + fy * STAGE_WIDTH_NUM * 2;
		switch ( _map_data[ idx ] ) {
		case 1://四角
		{
		Vector pos_f( fx, fy, fpos.z );
			if ( isCollisionToSquare( pos_f, fpos, radius ) ) {
				result = ( fpos - ( pos_f + Vector( WORLD_SCALE / 4, WORLD_SCALE / 4 ) ) ).normalize( ) * vec.getLength( );
			}
		}
			break;
		case 2://扇型
		{
			Vector pos_inside( fx - ( fx % 2 ) + 1, fy - ( fy % 2 ) + 1, fpos.z );
			if ( isCollisionToCircle( pos_inside, fpos, radius ) ) {
				result = ( pos - fpos ).normalize( ) * vec.getLength( );
			}
		}
			break;
		case 3://L字
		{
			Vector pos_outside( fx + ( fx % 2 ), fy + ( fy % 2 ), fpos.z );
			Vector pos_inside( fx - ( fx % 2 ) + 1, fy - ( fy % 2 ) + 1, fpos.z );
			if ( isCollisionToL( pos_outside, pos_inside, fpos, radius ) ) {
				result = ( pos_inside - pos_outside ).normalize( ) * vec.getLength( );
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
	_map_data = { };

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

bool AppStage::isOnStation( Vector pos ) {
	bool result = false;
	int x = ( int )( pos.x / WORLD_SCALE ) % STAGE_WIDTH_NUM;
	int y = ( int )( pos.y / WORLD_SCALE ) % STAGE_HEIGHT_NUM;
	int idx = x + y * STAGE_WIDTH_NUM;

	DATA data = getData( );
	int phase = getPhase( );
	if ( data.station[ phase ][ idx ] == _station_count ) {
		data.station[ phase ][ idx ] = 0;
		_station_count++;
		setData( data );
		result = true;
	}
	return result;
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

void AppStage::drawEarth( ) const {
	Vector adjust_pos = Vector( WORLD_SCALE * 2, WORLD_SCALE * 2 + WORLD_SCALE / 3, WORLD_SCALE / 6 );
	Drawer::ModelMDL model( adjust_pos, MDL_EARTH );
	_viewer->drawModelMDLTransfer( model );
}

void AppStage::drawWall( ) const {
	std::vector< Drawer::ModelMDL > walls = getWalls( );
	std::vector< Drawer::ModelMDL >::const_iterator ite = walls.begin( );
	while ( ite != walls.end( ) ) {
		_viewer->drawModelMDL( *ite );
		ite++;
	}
}

void AppStage::drawStation( ) const {
	DATA data = getData( );
	int phase = getPhase( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( data.station[ phase ][ i ] == _station_count ) {
			double x = double( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 3;
			double y = double( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2;
			_viewer->drawModelMDL( Drawer::ModelMDL( Vector( x, y, 0 ), MDL_STATION ) );
		}
	}
}

int AppStage::getStationCount( ) const {
	return _station_count;
}

void AppStage::loadPhase( ) {
	_station_count = 1;
	Stage::loadPhase( );
}

bool AppStage::isCollisionToSquare( Vector square_pos, Vector pos, double radius ) const {
	bool result = false;
	if ( square_pos.x - pos.x > STAGE_WIDTH_NUM ) {
		pos.x += STAGE_WIDTH_NUM * 2;
	}
	if ( pos.x - square_pos.x > STAGE_WIDTH_NUM ) {
		square_pos.x += STAGE_WIDTH_NUM * 2;
	}
	if ( square_pos.y - pos.y > STAGE_HEIGHT_NUM ) {
		pos.y += STAGE_HEIGHT_NUM * 2;
	}
	if ( pos.y - square_pos.y > STAGE_HEIGHT_NUM ) {
		square_pos.y += STAGE_HEIGHT_NUM * 2;
	}
	square_pos -= Vector( radius, radius );
	double size = WORLD_SCALE / 2 + radius * 2;
	if ( ( square_pos.x < pos.x ) &&
		 ( square_pos.x + size > pos.x ) &&
		 ( square_pos.y < pos.y ) &&
		 ( square_pos.y + size > pos.y ) ) {
		result = true;
	}
	return result;
}

bool AppStage::isCollisionToCircle( Vector circle_pos, Vector pos, double radius ) const {
	if ( circle_pos.x - pos.x > STAGE_WIDTH_NUM ) {
		pos.x += STAGE_WIDTH_NUM * 2;
	}
	if ( pos.x - circle_pos.x > STAGE_WIDTH_NUM ) {
		circle_pos.x += STAGE_WIDTH_NUM * 2;
	}
	if ( circle_pos.y - pos.y > STAGE_HEIGHT_NUM ) {
		pos.y += STAGE_HEIGHT_NUM * 2;
	}
	if ( pos.y - circle_pos.y > STAGE_HEIGHT_NUM ) {
		circle_pos.y += STAGE_HEIGHT_NUM * 2;
	}
	double circle_radius = WORLD_SCALE / 2;
	double distance = ( circle_pos - pos ).getLength( );
	return ( circle_radius + radius > distance );
}

bool AppStage::isCollisionToL( Vector pos_outside, Vector pos_inside, Vector pos, double radius ) const {
	if ( pos_outside.x - pos.x > STAGE_WIDTH_NUM ) {
		pos.x += STAGE_WIDTH_NUM * 2;
	}
	if ( pos.x - pos_outside.x > STAGE_WIDTH_NUM ) {
		pos_outside.x += STAGE_WIDTH_NUM * 2;
	}
	if ( pos_outside.y - pos.y > STAGE_HEIGHT_NUM ) {
		pos.y += STAGE_HEIGHT_NUM * 2;
	}
	if ( pos.y - pos_outside.y > STAGE_HEIGHT_NUM ) {
		pos_outside.y += STAGE_HEIGHT_NUM * 2;
	}
	if ( fabs( pos_outside.x - pos.x ) > radius * 2 ||
		 fabs( pos_outside.y - pos.y ) > radius * 2 ) {
		return false;
	}

	if ( pos_inside.x - pos.x > STAGE_WIDTH_NUM ) {
		pos.x += STAGE_WIDTH_NUM * 2;
	}
	if ( pos.x - pos_inside.x > STAGE_WIDTH_NUM ) {
		pos_inside.x += STAGE_WIDTH_NUM * 2;
	}
	if ( pos_inside.y - pos.y > STAGE_HEIGHT_NUM ) {
		pos.y += STAGE_HEIGHT_NUM * 2;
	}
	if ( pos.y - pos_inside.y > STAGE_HEIGHT_NUM ) {
		pos_inside.y += STAGE_HEIGHT_NUM * 2;
	}

	double distance = ( pos_inside - pos ).getLength( );
	double size = WORLD_SCALE / 2;
	return ( size - radius < distance );
}
