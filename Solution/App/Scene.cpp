#include "Scene.h"
#include "Application.h"
#include "Drawer.h"
#include "define.h"

const int FADE_SIZE = 256;

Scene::Scene( ) :
_fade_in_count( 0 ),
_fade_out_count( MAX_FADE_COUNT ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_FADE, "scene/fade_1.png" );
	drawer->loadGraph( GRAPH_FADE_BG, "scene/fade_bg.png" );
}


Scene::~Scene( ) {
}

void Scene::drawFadeIn( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );	

	int fade_size = 256;
	double blend = 1;
	blend = 1 - ( (double)_fade_in_count / 25 );
	if ( blend < 0 ) {
		blend = 0;
	}

	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( 0, 0, 0, 0, fade_size, fade_size, WIDTH, HEIGHT ), GRAPH_FADE, Drawer::BLEND_ALPHA, blend );
	drawer->setSprite( sprite );
}

void Scene::drawFadeOut( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );

	int fade_size = 256;
	double blend = 0;
	blend = 0 + ( (double)( MAX_FADE_COUNT - _fade_out_count ) / 25 );
	if ( blend > 1 ) {
		blend = 1;
	}
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( 0, 0, 0, 0, fade_size, fade_size, WIDTH, HEIGHT ), GRAPH_FADE, Drawer::BLEND_ALPHA, blend );
	drawer->setSprite( sprite );
}

void Scene::drawFadeBg( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( 0, 0, 0, 0, 256, 256, WIDTH, HEIGHT ), GRAPH_FADE_BG, Drawer::BLEND_ALPHA, 0.75 );
	drawer->setSprite( sprite );
}

void Scene::addFadeInCount( ) {
	_fade_in_count++;
}

void Scene::subFadeOutCount( ) {
	_fade_out_count--;
}

int Scene::getFadeInCount( ) const {
	return _fade_in_count;
}

int Scene::getFadeOutCount( ) const {
	return _fade_out_count;
}

void Scene::resetCount( ) {
	_fade_in_count = 0;
	_fade_out_count = MAX_FADE_COUNT;
}