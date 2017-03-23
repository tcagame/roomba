#include "Stage.h"
#include "Model.h"
#include "define.h"
#include "Crystal.h"

static const Vector MODEL_SIZE( 6, 6 );
static const std::string STAGE_FILE_NAME = "../Resource/Model/Stage/floor01.mdl";
static const Vector CRYSTAL_SCALE( 2, 2, 2 );

Stage::Stage( ) {
	_stage_data = { 
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0
	};
}

Stage::~Stage( ) {
}

void Stage::draw( ) {
	drawWireFrame( );
	drawCrystal( );
}

void Stage::drawWireFrame( ) {
	const int pitch = 10;
	DrawerPtr drawer = Drawer::getTask( );

	//Å@è∞ÇÃï`âÊ
	for ( int i = 0; i <= 10; i++ ) {
		Vector vec1 = Vector( 0, 0, 0 );
		Vector vec2 = Vector( 0, 100, 0 );
		Vector vec3 = Vector( 100, 0, 0 );
		Vector pitchX = Vector( ( i * pitch ), 0, 0 );
		Vector pitchY = Vector( 0, ( i * pitch ), 0 );
		drawer->drawLine( vec1 + pitchX, vec2 + pitchX );
		drawer->drawLine( vec1 + pitchY, vec3 + pitchY );
	}

	// óßëÃÇÃï`âÊ
	for ( int i = 0; i < STAGE_SIZE; i++ ) {
		if ( _stage_data[ i ] == 1 ) {
			Vector vec1 = Vector( ( i % 10 ) * pitch, ( i / 10 ) * pitch, 0 );
			Vector vec2 = vec1 + Vector( pitch, 0, 0 );
			Vector vec3 = vec1 + Vector( 0, pitch, 0 );
			Vector vec4 = vec1 + Vector( pitch, pitch, 0 );

			//ècÇÃï”
			drawer->drawLine( vec1, vec1 + Vector( 0, 0, pitch ) );
			drawer->drawLine( vec2, vec2 + Vector( 0, 0, pitch ) );
			drawer->drawLine( vec3, vec3 + Vector( 0, 0, pitch ) );
			drawer->drawLine( vec4, vec4 + Vector( 0, 0, pitch ) );
			//è„ñ 
			drawer->drawLine( vec1 + Vector( 0, 0, pitch ), vec2 + Vector( 0, 0, pitch ) );
			drawer->drawLine( vec2 + Vector( 0, 0, pitch ), vec4 + Vector( 0, 0, pitch ) );
			drawer->drawLine( vec3 + Vector( 0, 0, pitch ), vec4 + Vector( 0, 0, pitch ) );
			drawer->drawLine( vec3 + Vector( 0, 0, pitch ), vec1 + Vector( 0, 0, pitch ) );
		}
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

CrystalPtr Stage::getHittingCrystal( Vector pos0, Vector pos1 ) {
	CrystalPtr hitting = CrystalPtr( );
	//Ç†ÇΩÇ¡ÇƒÇ¢ÇÈÉNÉäÉXÉ^ÉãÇhittingÇ…ë„ì¸Ç∑ÇÈ
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