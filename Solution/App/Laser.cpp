#include "Laser.h"
#include "Model.h"
#include "Drawer.h"
#include "define.h"

const int PARTICLE_NUM = 100;
const double FAN_RADIUS = 0.1;
const int FAN_NUM = 16;

Laser::Laser( ) :
_particle( PARTICLE_NUM ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_LASER, "texture/laser.png" );

	_model = ModelPtr( new Model );
	_model->alloc( PARTICLE_NUM * FAN_NUM );
	_model->setPolygonNum( FAN_NUM );
}


Laser::~Laser( ) {
}

void Laser::update( const Vector& pos ) {
	Vector base[ FAN_NUM ];
	for ( int i = 0; i < FAN_NUM; i++ ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), PI2 * i / FAN_NUM );
		base[ i ] = mat.multiply( Vector( FAN_RADIUS, 0, 0 ) );
	}

	Model::VERTEX vertex[ 3 ];
	vertex[ 0 ].u = 0.5;
	vertex[ 0 ].v = 0;
	vertex[ 1 ].u = 0;
	vertex[ 1 ].v = 1.0;
	vertex[ 2 ].u = 1.0;
	vertex[ 2 ].v = 1.0;

	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		for ( int j = 0; j < FAN_NUM; j++ ) {
			int idx = ( i * FAN_NUM + j ) * 3;
			vertex[ 0 ].pos = pos + _particle[ i ];
			_model->set( idx + 0, vertex[ 0 ] );
			vertex[ 1 ].pos = pos + _particle[ i ] + base[ j ];
			_model->set( idx + 1, vertex[ 1 ] );
			vertex[ 2 ].pos = pos + _particle[ i ] + base[ ( j + 1 ) % FAN_NUM ];
			_model->set( idx + 2, vertex[ 2 ] );
		}
	}
}

void Laser::draw( ) const {
	Drawer::ModelSelf self;
	self.model = _model;
	self.graph = GRAPH_LASER;
	DrawerPtr drawer = Drawer::getTask( );
	drawer->setModelSelf( self );
}

