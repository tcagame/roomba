#include "Stage.h"
#include "Model.h"
#include "define.h"

static const Vector MODEL_SIZE( 6, 6 );
static const std::string STAGE_FILE_NAME = "../Resource/Model/Stage/floor01.mdl";

Stage::Stage( ) :
_width( 0 ),
_height( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadMDLModel( MDL::MDL_STAGE, "Model/Stage/floor01.mdl", "Model/Stage/floor01_DM.jpg" );
	loadMapCSV( );
	loadModelPos( );
}

Stage::~Stage( ) {
}

void Stage::draw( ) {
	DrawerPtr drawer = Drawer::getTask( );
	for ( int i = 0; i < _width * _height; i++ ) {
		MDL type = (MDL)_map_data[ i ];
		if ( type == MDL::MDL_NONE ) {
			continue;
		}
		Vector pos( i % _width * MODEL_SIZE.x, i / _height * MODEL_SIZE.y );
		Drawer::ModelMDL model = Drawer::ModelMDL( pos, MDL::MDL_STAGE );
		drawer->setModelMDL( model );
	}
}

void Stage::loadMapCSV( ) {
	//ファイルの読み込み
	const char* file_name = "../Resource/CSV/map.csv";
	FILE* fp;
	errno_t err = fopen_s( &fp, file_name, "r" );
	if ( err != 0 ) {
		return;
	}
	
	char buf[ 2048 ];

    //　カウントする
	while ( fgets( buf, 2048, fp ) != NULL ) {
		int width = 0;
		std::string str = buf;
		while ( true ) {
			width++;
			std::string::size_type index = str.find( "," );

			if ( index == std::string::npos ) {
				//","が見つからない場合
				_map_data.push_back( atoi( str.c_str( ) ) );
				break;
			}
			std::string substr = str.substr( 0, index );
			_map_data.push_back( atoi( substr.c_str( ) ) );
			str = str.substr( index + 1 );
		}

		if ( _height == 0 ) {
			_width = width;
		}
		_height++;
	}
	return;
}

void Stage::loadModelPos( ) {
	ModelPtr model( new Model );
	model->load( STAGE_FILE_NAME );
	int polygon_num = model->getPolygonNum( );

	for ( int i = 0; i < _width * _height; i++ ) {
		if ( _map_data[ i ] == 0 ) {
			_model_data[ i ] = ModelData( );
			continue;
		}
		Vector move( i % _width * MODEL_SIZE.x, i / _height * MODEL_SIZE.y );
		_model_data[ i ].polygon_num = polygon_num;
		model->translate( move );

		double max_x = 0;
		double max_y = 0;
		double min_x = 10000;
		double min_y = 10000;
		for ( int j = 0; j < polygon_num * 3; j++ ) {
			Vector pos = model->getPoint( j );
			_model_data[ i ].pos[ j ] = pos;
			if ( pos.x > max_x ) {
				max_x = pos.x;
			}
			if ( pos.y > max_y ) {
				max_y = pos.y;
			}
			if ( pos.x < min_x ) {
				min_x = pos.x;
			}
			if ( pos.y < min_y ) {
				min_y = pos.y;
			}
		}
		_model_data[ i ].max_x = max_x;
		_model_data[ i ].max_y = max_y;
		_model_data[ i ].min_x = min_x;
		_model_data[ i ].min_y = min_y;
	}
}

bool Stage::isCollision( Vector pos, Vector vec, Vector scale ) {
	bool ret = false;
	Vector check_pos = pos + vec;

	for ( int i = 0; i < _width * _height; i++ ) {
		if ( _map_data[ i ] == 0 ) {
			continue;
		}
		//モデルがある範囲にposが在るか確かめる
		if ( _model_data[ i ].max_x < check_pos.x - scale.x ) {
			continue;
		}  
		if ( _model_data[ i ].max_y < check_pos.y - scale.y ) {
			continue;
		}  
		if ( _model_data[ i ].min_x > check_pos.x + scale.x ) {
			continue;
		}  
		if ( _model_data[ i ].min_y > check_pos.y + scale.y ) {
			continue;
		}
		//あたっているか確かめる
		if ( vec.z > 0 ) {
			check_pos.z += scale.z;
		}
		if ( vec.z < 0 ) {
			check_pos.z -= scale.z;
		}
		ret = isCollisionModel( _model_data[ i ], check_pos );
		if ( ret ) {
			return true;
		}
	}
	return false;
}

bool Stage::isCollisionModel( ModelData model, Vector check_pos ) {
	Vector pos_p = check_pos;
	int polygon_idx = 0;
	for ( int i = 0; i < model.polygon_num; i++ ) {
		polygon_idx = i * 3;
		Vector pos_a = model.pos[ polygon_idx ];
		Vector pos_b = model.pos[ polygon_idx + 1 ];
		Vector pos_c = model.pos[ polygon_idx + 2 ];

		Vector ab = pos_a.sub( pos_b );
		Vector bp = pos_b.sub( pos_p );
		Vector bc = pos_b.sub( pos_c );
		Vector cp = pos_c.sub( pos_p );
		Vector ca = pos_c.sub( pos_a );
		Vector ap = pos_a.sub( pos_p );
		Vector c1 = ab.cross( bp );
		Vector c2 = bc.cross( cp );
		Vector c3 = ca.cross( ap );
		double dot1 = c1.dot( c2 );
		double dot2 = c1.dot( c3 );
		if ( dot1 > 0 && dot2 > 0 ) {
			return true;
		}
	}
	return false;
}