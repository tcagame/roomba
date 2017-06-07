#include "Laser.h"
#include "Model.h"
#include "Drawer.h"
#include "define.h"
#include "Camera.h"
#include "Crystal.h"

const int PARTICLE_NUM = 40 * 4;
const double FAN_RADIUS = 0.05;
const int FAN_NUM = 8;
const double RANGE_X = 3.0;
const double RANGE_Y = 0.03;
const double CRYSTAL_RADIUS = 0.1;

Laser::Laser( ) :
_particle( PARTICLE_NUM ),
_guide( PARTICLE_NUM ),
_index( 0 ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_LASER, "texture/laser.png" );

	_model = ModelPtr( new Model );
	_model->alloc( PARTICLE_NUM * FAN_NUM );

	for ( int i = 0; i < PARTICLE_NUM / 4; i++ ) {
		double x = RANGE_X * i * i;
		x /= ( PARTICLE_NUM / 4 ) * ( PARTICLE_NUM / 4 );
		double y = ( double )( PARTICLE_NUM / 4 - i ) * RANGE_Y / ( PARTICLE_NUM / 4 );
		_guide[ i ] = Vector( x, y, 0 );
	}
	
	for ( int i = 0; i < PARTICLE_NUM / 4; i++ ) {
		_guide[ i + PARTICLE_NUM / 4 ] = _guide[ PARTICLE_NUM / 4 - i - 1 ];
		_guide[ i + PARTICLE_NUM / 4 ].y *= -1;
	}

	for ( int i = 0; i < PARTICLE_NUM / 2; i++ ) {
		_guide[ i + PARTICLE_NUM / 2 ] = _guide[ i ];
		_guide[ i + PARTICLE_NUM / 2 ].x *= -1;
		_guide[ i + PARTICLE_NUM / 2 ].y *= -1;
	}
}

Laser::~Laser( ) {
}

void Laser::update( const Vector& pos, CameraConstPtr camera, const Vector& left, const Vector& right, CrystalConstPtr crystal ) {
	_index++;
	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		_particle[ i ] += ( _guide[ ( i + _index / 10 ) % PARTICLE_NUM ] - _particle[ i ] ) * 0.01;
	}

	Matrix posture;
	{
		Vector camera_dir = camera->getDir( ) * -1;
		Vector axis = camera_dir.cross( Vector( 0, 0, 1 ) );
		double angle = camera_dir.angle( Vector( 0, 0, 1 ) );
		posture = Matrix::makeTransformRotation( axis, angle );
	}

	Vector base[ FAN_NUM ];
	for ( int i = 0; i < FAN_NUM; i++ ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), PI2 * i / FAN_NUM ) * posture;
		base[ i ] = mat.multiply( Vector( FAN_RADIUS, 0, 0 ) );
	}

	Model::VERTEX vertex[ 3 ];
	vertex[ 0 ].u = 0.5;
	vertex[ 0 ].v = 0;
	vertex[ 1 ].u = 0;
	vertex[ 1 ].v = 1.0;
	vertex[ 2 ].u = 1.0;
	vertex[ 2 ].v = 1.0;
	
	Matrix rot; 
	{
		Vector dir = left - right;
		Vector axis = dir.cross( Vector( 1, 0, 0 ) );
		double angle = dir.angle( Vector( 1, 0, 0 ) );
		if ( !axis.isOrijin( ) ) {
			rot = Matrix::makeTransformRotation( axis, angle );
		}
	}

	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		Vector p = pos + rot.multiply( _particle[ i ] );

		for ( int j = 0; j < FAN_NUM; j++ ) {
			int idx = ( i * FAN_NUM + j ) * 3;
			vertex[ 0 ].pos = p;
			_model->set( idx + 0, vertex[ 0 ] );
			vertex[ 1 ].pos = p + base[ j ];
			_model->set( idx + 1, vertex[ 1 ] );
			vertex[ 2 ].pos = p + base[ ( j + 1 ) % FAN_NUM ];
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

