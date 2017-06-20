#include "AppStage.h"
#include "Application.h"
#include "Binary.h"
#include "define.h"
#include "Crystal.h"
#include "Viewer.h"
#include "Keyboard.h"
#include "Delivery.h"
#include "Timer.h"
#include "Roomba.h"

const double DELIVERY_POS_Z = FLOOR_POS_Z + WORLD_SCALE;
const double CRYSTAL_POS_Z = 0;

AppStage::AppStage( int stage_num, ViewerPtr viewer, TimerPtr timer, RoombaPtr roomba ) :
_delivery_count( 0 ),
_viewer( viewer ),
_finished( false ),
_timer( timer ),
_roomba( roomba ) {
	load( 0 );//0~2:通常 3:test_stage
	loadMapData( );
	int width = ( STAGE_WIDTH_NUM / FLOOR_CHIP_SIZE );
	int height = ( STAGE_HEIGHT_NUM / FLOOR_CHIP_SIZE );
	_floor.pos = Vector( );
	_floor.type = MDL_FLOOR;
}


AppStage::~AppStage( ) {
}

void AppStage::reset( ) {
	loadMapData( );
}

bool AppStage::isFinished( ) const {
	return _finished;
}

void AppStage::update( CameraPtr camera ) {
	updateCrystal( _timer );
	updateDelivery( camera );
	debug( );
}

void AppStage::draw( ) const {
	drawModel( );
	drawFloor( );
	drawWall( );
}

void AppStage::updateCrystal( TimerPtr timer ) {
	ApplicationPtr app = Application::getInstance( );
	int scr_width = app->getWindowWidth( );
	DrawerPtr drawer = Drawer::getTask( );
	std::list< CrystalPtr >::iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = (*ite);
		if ( !crystal ) {
			ite++;
			continue;
		}
		if ( crystal->isFinished( ) ) {
			crystal.~shared_ptr( );
			ite = _crystals.erase( ite );
			timer->addTime( );
			_roomba->setWaitCount( );
			continue;
		}
		AppStagePtr stage = std::dynamic_pointer_cast< AppStage >( shared_from_this( ) );
		collideCrystal( crystal );
		crystal->update( stage );
		ite++;
	}
}

void AppStage::collideCrystal( CrystalPtr crystal ) {
	std::list< CrystalPtr >::iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		CrystalPtr other = (*ite);
		if ( !other ) {
			ite++;
			continue;
		}
		if ( crystal != other ) {
			Vector other_vec = other->getVec( );
			Vector vec = crystal->adjustHitToCircle( other->getPos( ), other_vec );
			if ( vec.getLength( ) > 0.001 ) {
				other->setVec( vec.normalize( ) * REFLECTION_POWER );
				other->toBound( );
			}
		}
		ite++;
	}
}

void AppStage::updateDelivery( CameraPtr camera ) {
	DrawerPtr drawer = Drawer::getTask( );
	std::list< DeliveryPtr >::iterator ite = _deliverys.begin( );
	while ( ite != _deliverys.end( ) ) {
		DeliveryPtr delivery = (*ite);
		if ( !delivery ) {
			ite++;
			continue;
		}
		if ( delivery->isFinished( ) ) {
			delivery.~shared_ptr( );
			ite = _deliverys.erase( ite );
			continue;
		}
		delivery->update( camera );
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
		ite = _crystals.erase( ite );
	}
	_crystals.clear( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if (  getData( i ).crystal != 0 ) {
			Vector pos = Vector( ( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, ( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, CRYSTAL_POS_Z );
			pos += Vector( STAGE_WIDTH_NUM * WORLD_SCALE, STAGE_HEIGHT_NUM * WORLD_SCALE );
			_crystals.push_back( CrystalPtr( new Crystal( pos, MDL_CRYSTAL ) ) );
		}
	}
}


void AppStage::loadDelivery( ) {
	_delivery_count++;
	if ( _delivery_count > getMaxDeliveryNum( ) ) {
		_finished = true;
	}
	std::list< DeliveryPtr >::const_iterator ite = _deliverys.begin( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if (  getData( i ).delivery == _delivery_count ) {
			Vector pos = Vector( ( i % STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, ( i / STAGE_WIDTH_NUM ) * WORLD_SCALE + WORLD_SCALE / 2, DELIVERY_POS_Z );
			pos += Vector( STAGE_WIDTH_NUM * WORLD_SCALE, STAGE_HEIGHT_NUM * WORLD_SCALE );
			_deliverys.push_back( DeliveryPtr( new Delivery( pos ) ) );
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

void AppStage::drawDelivery( ) const {
	std::list< DeliveryPtr >::const_iterator ite = _deliverys.begin( );
	while ( ite != _deliverys.end( ) ) {
		if ( !(*ite) ) {
			ite++;
			continue;
		}
		(*ite)->draw( _viewer );
		ite++;
	}
}


CrystalPtr AppStage::getHittingCrystal( Vector& pos0, Vector& pos1, Vector& vec0, Vector& vec1 ) const {
	CrystalPtr hitting = CrystalPtr( );
	//あたっているクリスタルをhittingに代入する
	std::list< CrystalPtr >::const_iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = (*ite);
		if ( !crystal ) {
			ite++;
			continue;
		}
		if ( crystal->isHitting( pos0, pos1, vec0, vec1 ) ) {
			hitting = crystal;
		}
		ite++;
	}
	return hitting;
}

Vector AppStage::adjustCollisionToWall( Vector pos, Vector vec, const double radius ) {
	// ボールと壁の当たり判定
	Vector result = vec;
	bool collision = false;
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
		Vector pos_outside( fx + ( fx % 2 )	   , fy + ( fy % 2 )    , fpos.z );
		Vector pos_inside ( fx - ( fx % 2 ) + 1, fy - ( fy % 2 ) + 1, fpos.z );
		Vector f_idx_pos( fx, fy, fpos.z );
		switch ( _map_data[ idx ] ) {
		case 1://四角
			if ( isCollisionToSquare( f_idx_pos, fpos, radius ) ) {
				if ( pos.x < f_idx_pos.x ||
					 pos.x > ( f_idx_pos.x + WORLD_SCALE / 2 ) ) {
					result.x = vec.x * -1;
				} else 	if ( pos.y < f_idx_pos.y ||
					 pos.y > ( f_idx_pos.y + WORLD_SCALE / 2 ) ) {
					result.y = vec.y * -1;
				} else {
					result = vec * -1;
				}
				collision = true;
			}
			break;
		case 2://扇型
			if ( isCollisionToCircle( pos_inside, fpos, radius ) ) {
				result = ( pos - pos_inside );
				collision = true;
			}
			break;
		case 3://L字
			if ( isCollisionToL( pos_outside, pos_inside, fpos, radius ) ) {
				result = ( pos - pos_outside );
				collision = true;
			}
			break;
		}
	}
	if ( collision ) {
		result.z = 0;
		result = result.normalize( ) * REFLECTION_POWER;
	}
	return result;
}

Vector AppStage::adjustCollisionToCrystal( Vector pos, Vector& vec, const double radius ) {
	Vector result;
	std::list< CrystalPtr >::const_iterator ite = _crystals.begin( );
	while ( ite != _crystals.end( ) ) {
		CrystalPtr crystal = (*ite);
		if ( !crystal ) {
			ite++;
			continue;
		}
		crystal->shiftPos( pos );
		Vector adjust = crystal->adjustHitToCircle( pos, vec, radius );
		result += adjust.normalize( ) * REFLECTION_POWER;
		ite++;
	}
	return result;
}


void AppStage::loadMapData( ) {
	const int OFFSET_X[ 8 ] = { -1, 1, -1, 1, 0, 0, -1, 1 };
	const int OFFSET_Y[ 8 ] = { -1, -1, 1, 1, -1, 1, 0, 0 };
	_map_data = { };

	std::array< Stage::DATA, STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM > data = getData( );
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		int x = i % STAGE_WIDTH_NUM;
		int y = i / STAGE_WIDTH_NUM;
		int type = data[ i ].wall;
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
				if ( data[ idx0 ].wall == 0 && data[ idx1 ].wall == 0 ) {
					_map_data[ map_idx ] = 2;
				}
			}
			if ( type == 0 ) {
				if ( data[ idx0 ].wall == 1 && data[ idx1 ].wall == 1 ) {
					_map_data[ map_idx ] = 3;
				}
			}
		}
	}
}

void AppStage::loadWall( ) {
	for ( int i = 0; i < WALL_DIV_SIZE; i++ ) {
		_walls[ i ].pos = Vector( );
		_walls[ i ].type = MDL_WALL_0 + i;
	}
}

bool AppStage::isOnDelivery( Vector& pos ) {
	bool result = false;
	std::list< DeliveryPtr >::iterator ite = _deliverys.begin( );
	while ( ite != _deliverys.end( ) ) {
		if ( !(*ite) ) {
			ite++;
			continue;
		}
		DeliveryPtr delivery = (*ite);
		if ( delivery->isHaveCrystal( ) ) {
			ite++;
			continue;
		}
		Vector delivery_pos = delivery->getPos( );
		delivery_pos.z = pos.z;
		adjustPos( delivery_pos, pos );
		Vector distance = delivery_pos - pos;
		if ( distance.getLength( ) > 0.8 ) {
			ite++;
			continue;
		}
		delivery->setCrystal( pos );
		loadDelivery( );
		result = true;
		break;
	}
	return result;
}

std::list< CrystalPtr > AppStage::getCrystalList( ) const {
	return _crystals;
}

void AppStage::drawFloor( ) const {
	_viewer->drawModelMDLMulti( _floor );
}

void AppStage::drawWall( ) const {
	for ( int i = 0; i < WALL_DIV_SIZE; i++ ) {
		_viewer->drawModelMDLMulti( _walls[ i ] );
	}
}

int AppStage::getDeliveryCount( ) const {
	return _delivery_count;
}

bool AppStage::isCollisionToSquare( Vector& square_pos, Vector pos, double radius ) const {
	bool result = false;
	adjustPos( square_pos, pos );
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
	adjustPos( pos, circle_pos );
	double circle_radius = WORLD_SCALE / 2;
	double distance = ( circle_pos - pos ).getLength( );
	return ( circle_radius + radius > distance );
}

bool AppStage::isCollisionToL( Vector pos_outside, Vector pos_inside, Vector pos, double radius ) const {
	adjustPos( pos, pos_outside );
	if ( fabs( pos_outside.x - pos.x ) > WALL_SIZE.x ||
		 fabs( pos_outside.y - pos.y ) > WALL_SIZE.y ) {
		return false;
	}

	adjustPos( pos, pos_inside );

	//逆側からの距離で調べる
	double distance = ( pos_inside - pos ).getLength( );
	double max_distance = WALL_SIZE.x * 2 - radius;
	return ( max_distance < distance );
}

void AppStage::adjustPos( Vector& pos, Vector& base_pos ) const {
	while ( pos.x - base_pos.x > STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		pos.x -= STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.x - base_pos.x < -STAGE_WIDTH_NUM * WORLD_SCALE / 2 ) {
		pos.x += STAGE_WIDTH_NUM * WORLD_SCALE;
	}
	while ( pos.y - base_pos.y > STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		pos.y -= STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
	while ( pos.y - base_pos.y < -STAGE_HEIGHT_NUM * WORLD_SCALE / 2 ) {
		pos.y += STAGE_HEIGHT_NUM * WORLD_SCALE;
	}
}
