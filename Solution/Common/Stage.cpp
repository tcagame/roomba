#include "Stage.h"
#include "Model.h"
#include "Crystal.h"
#include "define.h"
#include <array>
#include "Application.h"
#include "Binary.h"
#include "Mouse.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Roomba.h"

static const double MODEL_SIZE = 4;
static const int CURSOR_WAIT_TIME = 2;

Stage::Stage( ) :
_phase( -1 ),
_finished( false ),
_cursor_x( 0 ),
_cursor_y( 0 ),
_count( 0 ) {
	loadData( 3 );//0~2:通常 3:test_stage
	//クリスタル
	loadEarth( );
	loadWall( );
	loadPhase( );
}

Stage::~Stage( ) {
}

void Stage::update( RoombaPtr roomba ) {
#if _DEBUG
	KeyboardPtr keyboard = Keyboard::getTask( );
	for ( int i = 0; i < MAX_STAGE; i++ ) {
		std::string key = std::to_string( i );
		if ( keyboard->isPushKey( key ) ) {
			loadData( i );
			reset( );
			roomba->reset( );
		}
	}
#endif
	updateCrystal( roomba );
}

void Stage::updateCrystal( RoombaPtr roomba ) {
	if ( _crystals.size( ) == 0 ) {
		loadPhase( );
	}
	ApplicationPtr app = Application::getInstance( );
	int scr_width = app->getWindowWidth( );
	DrawerPtr drawer = Drawer::getTask( );
	std::list< CrystalPtr >::const_iterator ite = _crystals.begin( );
	int num = 0;
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = (*ite);
		if ( !crystal ) {
			ite++;
			continue;
		}
		crystal->update( shared_from_this( ), roomba );
		Vector pos = crystal->getPos( );
		drawer->drawString( scr_width - 280, num * 20, "[クリスタル%2d] x:%04.1f y:%04.1f", num, pos.x, pos.y );
		num++;
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
	//drawCollisionLine( );
	drawCrystal( );
	drawStation( );
}

void Stage::drawBackground( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setModelMDL( Drawer::ModelMDL( Vector( ), MDL_BG ) );
}

void Stage::drawEarth( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		drawer->setModelMDL( _earth[ i ] );
	}
}

void Stage::drawWall( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	std::vector< Drawer::ModelMDL >::const_iterator ite = _walls.begin( );
	while ( ite != _walls.end( ) ) {
		drawer->setModelMDL( (*ite) );
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

void Stage::drawStation( ) const {
    DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _data.station[ _phase ][ i ] != 0 ) {
			MDL mdl = (MDL)( (int)MDL_STATION_0 + _data.station[ _phase ][ i ] - 1 );
			double x = double( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 3;
			double y = double( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2;
			Drawer::ModelMDL model( Vector( x, y, 0 ), mdl );
			drawer->setModelMDL( model );
		}
	}
}

void Stage::loadData( int stage_num ) {
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
	_data = getFileData( filename );
}

void Stage::loadCrystal( ) {
	_crystals.clear( );
	

	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _data.crystal[ _phase ][ i ] != 0 ) {
			Vector pos = Vector( ( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, ( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, -WORLD_SCALE );
			MDL mdl = (MDL)( (int)MDL_CRYSTAL_0 + _data.crystal[ _phase ][ i ] - 1 );
			_crystals.push_back( CrystalPtr( new Crystal( pos, mdl ) ) );
		}
	}
}

void Stage::drawCollisionLine( ) const {
	if ( _phase >= MAX_PHASE ) {
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
        if ( _data.wall[ i ] == 1 ) {
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

void Stage::loadEarth( ) {
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		Vector pos( x * WORLD_SCALE + WORLD_SCALE / 2, y * WORLD_SCALE + WORLD_SCALE / 2, 0 );
		Vector adjust_pos = Vector( WORLD_SCALE + WORLD_SCALE / 2, WORLD_SCALE + WORLD_SCALE / 3 );
		_earth[ i ] = Drawer::ModelMDL( pos + adjust_pos, MDL_EARTH );
	}
}

void Stage::loadWall( ) {
	_walls.clear( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * 2 * STAGE_HEIGHT_NUM * 2; i++ ) {
		_map_data[ i ] = 0;
	}
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
	_walls.clear( );

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
		//壁生成
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		Vector pos( x * WORLD_SCALE + WORLD_SCALE / 2, y * WORLD_SCALE + WORLD_SCALE / 2, 0 );
		Vector adjust_pos;
		int type = _data.wall[ i ];
		if ( type != 0 && type != 1 ) {
			continue;
		}
		unsigned char flag = 0;
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
				if ( _data.wall[ idx0 ] == 0 && _data.wall[ idx1 ] == 0 ) {
					_map_data[ map_idx ] = 2;
					flag |= 1 << j;
				}
			}
			if ( type == 0 ) {
				if ( _data.wall[ idx0 ] == 1 && _data.wall[ idx1 ] == 1 ) {
					flag |= 1 << j;
					_map_data[ map_idx ] = 3;
				}
			}
		}
		if ( type == 1 ) {
			if ( flag == 1 ||
				 flag == 3 ||
				 flag == 5 ||
				 flag == 15 ) {
				adjust_pos = Vector( -WORLD_SCALE / 2, 0, 0 );
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
}

Vector Stage::adjustCollisionToWall( Vector pos, Vector vec, const double radius ) {
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

Vector Stage::adjustCollisionToCrystal( Vector pos, Vector vec, const double radius ) {
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

bool Stage::isFinished( ) const {
	return _finished;
}

CrystalPtr Stage::getHittingCrystal( Vector pos0, Vector pos1 ) const {
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
	_phase = -1;
	_finished = false;
	loadEarth( );
	loadWall( );
	loadPhase( );
}

void Stage::load( ) {
	ApplicationPtr app = Application::getInstance( );
	std::string filename = "../Resource/Map/" + app->inputString( 0, 40 );
	_data = getFileData( filename );
	reset( );
}

Stage::DATA Stage::getFileData( std::string filename ) const {
	ApplicationPtr app = Application::getInstance( );
	BinaryPtr data( new Binary );
	if ( filename.find( ".stage" ) == std::string::npos ) {
		filename += ".stage";
	}
	if ( !app->loadBinary( filename, data ) ) {
		return DATA( );
	}
	return *(DATA*)data->getPtr( );
}

void Stage::save( ) const {
	BinaryPtr data = BinaryPtr( new Binary );
	data->ensure( sizeof( _data ) );
	data->write( (void*)&_data, sizeof( _data ) );
	ApplicationPtr app = Application::getInstance( );
	std::string filename = "../Resource/Map/" + app->inputString( 0, 40 );
	if ( filename.find( ".stage" ) == std::string::npos ) {
		filename += ".stage";
	}
	app->saveBinary( filename, data );
}

void Stage::setPhase( int phase ) {
	if ( phase < 0 || phase >= MAX_PHASE ) {
		return;
	}
	_phase = phase;
	loadCrystal( );
}

void Stage::drawEditor( ) const {
	DrawerPtr drawer = Drawer::getTask( );
	Vector cursor_pos( _cursor_x * WORLD_SCALE + WORLD_SCALE / 2, _cursor_y * WORLD_SCALE  + WORLD_SCALE / 4 );
	drawer->setModelMDL( Drawer::ModelMDL( cursor_pos, MDL_CURSOR ) );
	drawer->drawString( 0, 60, "WAVE:%d", _phase );
	drawer->drawString( 0, 0, "壁編集:1　クリスタル編集:2　ステーション編集:3　セーブ:F1　ロード:F2　フェーズ変更:テンキー" );


	for ( int i = 0; i <= STAGE_WIDTH_NUM; i++ ) {
		double x = MODEL_SIZE / WORLD_SCALE * (double)i;
		Vector pos0( x, 0 );
		Vector pos1( x, MODEL_SIZE / WORLD_SCALE * STAGE_HEIGHT_NUM );
		drawer->drawLine( pos0, pos1 );
	}
	for ( int i = 0; i <= STAGE_HEIGHT_NUM; i++ ) {
		double y = MODEL_SIZE / WORLD_SCALE * (double)i;
		Vector pos0( 0, y );
		Vector pos1( MODEL_SIZE / WORLD_SCALE * STAGE_WIDTH_NUM, y );
		drawer->drawLine( pos0, pos1 );
	}

}

void Stage::editWall( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:壁配置" );
	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( _data.wall[ idx ] == 0 ) {
			_data.wall[ idx ] = 1;
			loadWall( );
		}
	}
	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( _data.wall[ idx ] == 1 ) {
			_data.wall[ idx ] = 0;
			loadWall( );
		}
	}
}

void Stage::editCrystal( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:クリスタル配置" );
	
	bool num[ MAX_LINK ];
	for ( int i = 0; i < MAX_LINK; i++ ) {
		num[ i ] = false;
	}

	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _data.crystal[ _phase ][ i ] > 0 ) {
			num[ _data.crystal[ _phase ][ i ] - 1 ] = true;
		}
	}
	int number = 0;
	for ( int i = 0; i < MAX_LINK; i++ ) {
		if ( !num[ i ] ) {
			number = i + 1;
			break;
		}
	}

	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) && number != 0 ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( _data.crystal[ _phase ][ idx ] == 0 ) {
			_data.crystal[ _phase ][ idx ] = number;
			loadCrystal( );
		}
	}
	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( _data.crystal[ _phase ][ idx ] != 0 ) {
			_data.crystal[ _phase ][ idx ] = 0;
			loadCrystal( );
		}
	}
}

void Stage::editStation( ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->drawString( 0, 80, "MODE:ステーション配置" );
	bool num[ MAX_LINK ] = { false };
	for ( int i = 0; i < MAX_LINK; i++ ) {
		num[ i ] = false;
	}

	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _data.station[ _phase ][ i ] ) {
			num[ _data.station[ _phase ][ i ] - 1 ] = true;
		}
	}
	int number = 0;
	for ( int i = 0; i < MAX_LINK; i++ ) {
		if ( !num[ i ] ) {
			number = i + 1;
			break;
		}
	}

	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( ( mouse->isHoldLeftButton( ) || keyboard->isHoldKey( "SPACE" ) ) && number != 0 ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( _data.station[ _phase ][ idx ] == 0 ) {
			_data.station[ _phase ][ idx ] = number;
		}
	}

	if ( mouse->isHoldRightButton( ) || keyboard->isHoldKey( "BACK_SPACE" ) ) {
		int idx = _cursor_y * STAGE_WIDTH_NUM + _cursor_x;
		if ( idx < 0 || idx > STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM ) {
			return;
		}
		if ( _data.station[ _phase ][ idx ] != 0 ) {
			_data.station[ _phase ][ idx ] = 0;
		}
	}
}

void Stage::updateCursor( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	int cursor_x = 0;
	int cursor_y = 0;
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		cursor_y++;
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		cursor_y--;
	}
	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		cursor_x++;
	}
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
		cursor_x--;
	}
	if ( cursor_x != 0 || cursor_y != 0 ) {
		_count++;
		if ( _count % CURSOR_WAIT_TIME == 0 ) {
			_cursor_x += cursor_x;
			_cursor_y += cursor_y;
		}
	} else {
		_count = 0;
	}
}