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
	_stage_num = stage_num;
	std::string filename = "../Resource/Map/" + std::to_string( _stage_num ) + ".stage";
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

void Stage::loadCrystal( ) {

}

void Stage::loadDelivery( ) {

}

void Stage::loadWall( ) {

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

int Stage::getStageNum( ) const {
	return _stage_num;
}