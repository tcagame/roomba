#include "Stage.h"
#include "Model.h"
#include "define.h"
#include "Crystal.h"

static const Vector MODEL_SIZE( 6, 6 );
static const std::string STAGE_FILE_NAME = "../Resource/Model/Stage/floor01.mdl";
static const Vector CRYSTAL_SCALE( 2, 2, 2 );
static const int PITCH = 10;

Stage::Stage( ) {
	Matrix mat = Matrix( );
	mat = mat.makeTransformScaling( CRYSTAL_SCALE );
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadMDLModel( MDL::MDL_CRYSTAL, "Model/Crystal/crystal.mdl", "Model/Crystal/crystal.jpg", mat );
	_crystals.push_back( CrystalPtr( new Crystal( Vector( 5, 5, 1 ) ) ) );
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
}

Stage::~Stage( ) {
}

void Stage::update( ) {
	updateCrystal( );
}

void Stage::updateCrystal( ) {
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
	drawWireFrame( );
	drawCrystal( );
}

void Stage::drawWireFrame( ) {
	DrawerPtr drawer = Drawer::getTask( );

	//　床の描画
	for ( int i = 0; i <= 10; i++ ) {
		Vector vec1 = Vector( 0, 0, 0 );
		Vector vec2 = Vector( 0, 100, 0 );
		Vector vec3 = Vector( 100, 0, 0 );
		Vector pitchX = Vector( ( i * PITCH ), 0, 0 );
		Vector pitchY = Vector( 0, ( i * PITCH ), 0 );
		drawer->drawLine( vec1 + pitchX, vec2 + pitchX );
		drawer->drawLine( vec1 + pitchY, vec3 + pitchY );
	}

	// 立体の描画
	for ( int i = 0; i < STAGE_WIDTH_NUM * STAGE_HEIGHT_NUM; i++ ) {
		if ( _stage_data[ i ] == 1 ) {
			Vector vec1 = Vector( ( i % STAGE_WIDTH_NUM ) * PITCH, ( i / STAGE_WIDTH_NUM ) * PITCH, 0 );
			Vector vec2 = vec1 + Vector( PITCH, 0, 0 );
			Vector vec3 = vec1 + Vector( 0, PITCH, 0 );
			Vector vec4 = vec1 + Vector( PITCH, PITCH, 0 );

			//縦の辺
			drawer->drawLine( vec1, vec1 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec2, vec2 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec3, vec3 + Vector( 0, 0, PITCH ) );
			drawer->drawLine( vec4, vec4 + Vector( 0, 0, PITCH ) );
			//上面
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