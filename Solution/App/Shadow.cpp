#include "Shadow.h"
#include "Drawer.h"
#include "define.h"
#include "Model.h"

Shadow::Shadow( ) {
	_model = ModelPtr( new Model );
	_model->alloc( MAX_SHADOW_NUM * 2 );
}

Shadow::~Shadow( ) {
}

void Shadow::set( const Vector& pos ) {
	_pos[ _num ] = pos;
	_num++;
}

void Shadow::update( ) {
	for ( int i = 0; i < _num; i++ ) {
		Model::VERTEX vertex[ 4 ];
		vertex[ 0 ].pos = _pos[ i ] + Vector( -0.5, -0.5 );
		vertex[ 1 ].pos = _pos[ i ] + Vector(  0.5, -0.5 );
		vertex[ 2 ].pos = _pos[ i ] + Vector( -0.5,  0.5 );
		vertex[ 3 ].pos = _pos[ i ] + Vector(  0.5,  0.5 );

		vertex[ 0 ].pos.z = 0.1;
		vertex[ 1 ].pos.z = 0.1;
		vertex[ 2 ].pos.z = 0.1;
		vertex[ 3 ].pos.z = 0.1;

		vertex[ 0 ].u = 0;
		vertex[ 0 ].v = 0;
		vertex[ 1 ].u = 1;
		vertex[ 1 ].v = 0;
		vertex[ 2 ].u = 0;
		vertex[ 2 ].v = 1;
		vertex[ 3 ].u = 1;
		vertex[ 3 ].v = 1;


		_model->set( i * 6 + 0, vertex[ 0 ] );
		_model->set( i * 6 + 1, vertex[ 1 ] );
		_model->set( i * 6 + 2, vertex[ 2 ] );
		_model->set( i * 6 + 3, vertex[ 1 ] );
		_model->set( i * 6 + 4, vertex[ 3 ] );
		_model->set( i * 6 + 5, vertex[ 2 ] );
	}

	_model->setPolygonNum( _num * 2 );
	_num = 0;
}

void Shadow::draw( ) const {
	Drawer::ModelSelf self;
	self.model = _model;
	self.graph = GRAPH_SHADOW;
	self.add = false;

	DrawerPtr drawer = Drawer::getTask( );
	drawer->setModelSelf( self );
}