#include "Stage.h"
#include "Model.h"
#include "define.h"
#include "Crystal.h"

static const Vector MODEL_SIZE( 6, 6 );
static const std::string STAGE_FILE_NAME = "../Resource/Model/Stage/floor01.mdl";
static const Vector CRYSTAL_SCALE( 2, 2, 2 );
static const int PITCH = 10;

Stage::Stage( ) :
_wave( 0 ),
_finished( false ) {
	Matrix mat = Matrix( );
	mat = mat.makeTransformScaling( CRYSTAL_SCALE );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadMDLModel( MDL::MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", mat );
	_stage_data = { 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0
	};
	_waves[ 0 ] = {
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	_waves[ 1 ] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	_waves[ 2 ] = {
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	loadCrystalData( );
}

Stage::~Stage( ) {
}

void Stage::update( ) {
	updateCrystal( );
	updateTimer( );
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

void Stage::updateTimer( ) {

}

void Stage::draw( ) {
	drawWireFrame( );
	drawCrystal( );
}

void Stage::drawWireFrame( ) {
	DrawerPtr drawer = Drawer::getTask( );

	//Å@è∞ÇÃï`âÊ
	for ( int i = 0; i <= 10; i++ ) {
		Vector vec1 = Vector( 0, 0, 0 );
		Vector vec2 = Vector( 0, 100, 0 );
		Vector vec3 = Vector( 100, 0, 0 );
		Vector pitchX = Vector( ( i * PITCH ), 0, 0 );
		Vector pitchY = Vector( 0, ( i * PITCH ), 0 );
		drawer->drawLine( vec1 + pitchX, vec2 + pitchX );
		drawer->drawLine( vec1 + pitchY, vec3 + pitchY );
	}

	// óßëÃÇÃï`âÊ
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _stage_data[ i ] == 1 ) {
			Vector vec1 = Vector( ( i % STAGE_WIDTH_NUM ) * PITCH, ( i / STAGE_WIDTH_NUM ) * PITCH, 0 );
			Vector vec2 = vec1 + Vector( PITCH, 0, 0 );
			Vector vec3 = vec1 + Vector( 0, PITCH, 0 );
			Vector vec4 = vec1 + Vector( PITCH, PITCH, 0 );

			//ècÇÃï”
			drawer->drawLine( vec1, vec1 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec2, vec2 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec3, vec3 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec4, vec4 + Vector( 0, 0, PITCH ) );
			//è„ñ 
			drawer->drawLine( vec1 + Vector( 0, 0, PITCH ), vec2 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec2 + Vector( 0, 0, PITCH ), vec4 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec3 + Vector( 0, 0, PITCH ), vec4 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec3 + Vector( 0, 0, PITCH ), vec1 + Vector( 0, 0, PITCH ) );
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

void Stage::loadCrystalData( ) {
	if ( _wave >= MAX_WAVE ) {
		_finished = true;
		return;
	}
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _waves[ _wave ][ i ] == 1 ) {
			Vector pos = Vector( ( i % STAGE_WIDTH_NUM ) * PITCH + PITCH / 2, ( i / STAGE_WIDTH_NUM ) * PITCH + PITCH / 2, 1 );
			_crystals.push_back( CrystalPtr( new Crystal( pos ) ) );
		}
	}
	_wave++;
}

bool Stage::isCollisionWall( Vector pos ) {
	// É{Å[ÉãÇ∆ï«ÇÃìñÇΩÇËîªíË
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

