#include "Laser.h"
#include "Model.h"
#include "Drawer.h"
#include "define.h"
#include "Camera.h"
#include "Crystal.h"

const int PARTICLE_NUM = 40 * 4;
const int HOLD_NUM = 20;
const double FAN_RADIUS = 0.05;
const int FAN_NUM = 8;
const double RANGE_X = 3.0;
const double RANGE_Y = 0.03;
const double CRYSTAL_RADIUS = 0.2;

Laser::Laser( ) :
_particle( PARTICLE_NUM ),
_guide( PARTICLE_NUM ),
_index( 0 ),
_hold_r( 0 ),
_showing( false ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_LASER, "texture/laser.png" );

	_model_line = ModelPtr( new Model );
	_model_line->alloc( PARTICLE_NUM * FAN_NUM );
	
	_model_hold = ModelPtr( new Model );
	_model_hold->alloc( HOLD_NUM * FAN_NUM );

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

void Laser::show( bool showing ) {
	if ( showing == _showing ) {
		return;
	}
	_showing = showing;

	if ( _showing ) {
		reset( );
	}
}

void Laser::reset( ) {
	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		_particle[ i ] = Vector( );
	}
}

void Laser::update( const Vector& pos, CameraConstPtr camera, const Vector& left, const Vector& right, CrystalConstPtr crystal ) {
	_index = ( _index + 3 ) % PARTICLE_NUM;
	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		_particle[ i ] += ( _guide[ ( i + _index / 10 ) % PARTICLE_NUM ] - _particle[ i ] ) * 0.02;
	}

	Vector base[ FAN_NUM ];
	{
		Matrix posture;
		Vector camera_dir = camera->getDir( ) * -1;
		Vector axis = camera_dir.cross( Vector( 0, 0, 1 ) );
		double angle = camera_dir.angle( Vector( 0, 0, 1 ) );
		posture = Matrix::makeTransformRotation( axis, angle );

		for ( int i = 0; i < FAN_NUM; i++ ) {
			Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), PI2 * i / FAN_NUM ) * posture;
			base[ i ] = mat.multiply( Vector( FAN_RADIUS, 0, 0 ) );
		}
	}

	Model::VERTEX vertex[ 3 ];
	vertex[ 0 ].u = 0.5;
	vertex[ 0 ].v = 0;
	vertex[ 1 ].u = 0;
	vertex[ 1 ].v = 1.0;
	vertex[ 2 ].u = 1.0;
	vertex[ 2 ].v = 1.0;
	
	_holding = false;
	Vector hold_pos;
	if ( crystal ) {
		_holding = true;
		hold_pos = crystal->getPos( );
		hold_pos.z = pos.z;
	}

	if ( _holding ) {
		_hold_r += 0.01;

		for ( int i = 0; i < HOLD_NUM; i++ ) {
			Matrix rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), _hold_r + PI2 * i / HOLD_NUM );
			Vector p = hold_pos + rot.multiply( Vector( CRYSTAL_RADIUS, 0, 0 ) );

			for ( int j = 0; j < FAN_NUM; j++ ) {
				int idx = ( i * FAN_NUM + j ) * 3;
				vertex[ 0 ].pos = p;
				_model_hold->set( idx + 0, vertex[ 0 ] );
				vertex[ 1 ].pos = p + base[ j ];
				_model_hold->set( idx + 1, vertex[ 1 ] );
				vertex[ 2 ].pos = p + base[ ( j + 1 ) % FAN_NUM ];
				_model_hold->set( idx + 2, vertex[ 2 ] );
			}
		}
	}

	{
		Matrix rot;
		Vector dir = left - right;
		Vector axis = dir.cross( Vector( 1, 0, 0 ) );
		double angle = dir.angle( Vector( 1, 0, 0 ) );
		if ( !axis.isOrijin( ) ) {
			rot = Matrix::makeTransformRotation( axis, angle );
		}

		for ( int i = 0; i < PARTICLE_NUM; i++ ) {
			Vector p = pos + rot.multiply( _particle[ i ] );

			if ( _holding ) {
				Vector vec = p - hold_pos;
				if ( vec.getLength( ) < CRYSTAL_RADIUS ) {
					p = hold_pos + vec.normalize( ) * CRYSTAL_RADIUS;
				}
			}

			for ( int j = 0; j < FAN_NUM; j++ ) {
				int idx = ( i * FAN_NUM + j ) * 3;
				vertex[ 0 ].pos = p;
				_model_line->set( idx + 0, vertex[ 0 ] );
				vertex[ 1 ].pos = p + base[ j ];
				_model_line->set( idx + 1, vertex[ 1 ] );
				vertex[ 2 ].pos = p + base[ ( j + 1 ) % FAN_NUM ];
				_model_line->set( idx + 2, vertex[ 2 ] );
			}
		}
	}
}

void Laser::draw( ) const {
	if ( !_showing ) {
		return;
	}

	DrawerPtr drawer = Drawer::getTask( );
	{
		Drawer::ModelSelf self;
		self.model = _model_line;
		self.graph = GRAPH_LASER;
		self.add = true;
		self.z_buffer = false;
		drawer->setModelSelf( self );
	}
	
	if ( _holding ) {
		Drawer::ModelSelf self;
		self.model = _model_hold;
		self.graph = GRAPH_LASER;
		self.add = true;
		self.z_buffer = false;
		drawer->setModelSelf( self );
	}
}

