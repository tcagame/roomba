#pragma once
#include <vector>
#include "Drawer.h"

class Stage {
public:
	Stage( );
	virtual ~Stage( );
public:
	void draw( );
	bool isCollision( Vector pos, Vector vec, Vector scale );
private:
	static const int MAX_POLYGON_NUM = 4096;
	static const int MAX_MODEL_NUM = 1024;
private:
	struct ModelData {
		std::array< Vector, MAX_POLYGON_NUM > pos;
		int polygon_num;
		double max_x;
		double max_y;
		double min_x; 
		double min_y;
		ModelData( ) {
			this->polygon_num = 0;
			this->max_x = 0;
			this->max_y = 0;
			this->min_x = 0;
			this->min_y = 0;
		};
	};
private:
	void loadMapCSV( );
	void loadModelPos( );
	bool isCollisionModel( ModelData model, Vector check_pos );
private:
	std::vector< int > _map_data;
	std::array< ModelData, MAX_MODEL_NUM > _model_data;
	int _width;
	int _height;
};

