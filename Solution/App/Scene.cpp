#include "Scene.h"
#include "Application.h"
#include "Drawer.h"
#include "define.h"

const int FADE_SIZE = 256;

Scene::Scene( ) :
_fade_in_count( 0 ),
_fade_out_count( MAX_FADE_COUNT ) {
	DrawerPtr drawer = Drawer::getTask( );
	drawer->loadGraph( GRAPH_FADE, "scene/fade.png" );
}


Scene::~Scene( ) {
}

void Scene::drawFadeIn( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );	
	const int idx = _fade_in_count / ANIME_FLAME;
	int tx = idx % 6;
	int ty = idx / 6;
	if ( idx >= 32 ) {
		ty = 5;
		if ( tx > 2 ) {
			tx = 2;
		}
	}
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( 0, 0, tx * FADE_SIZE, ty * FADE_SIZE, FADE_SIZE, FADE_SIZE, WIDTH, HEIGHT ), GRAPH_FADE );
	drawer->setSprite( sprite );
}

void Scene::drawFadeOut( ) const {
	ApplicationPtr app = Application::getInstance( );
	const int WIDTH = app->getWindowWidth( );
	const int HEIGHT = app->getWindowHeight( );
	const int idx = _fade_out_count / ANIME_FLAME;
	int tx = idx % 6;
	int ty = idx / 6;
	if ( idx >= 32 ) {
		ty = 5;
		if ( tx > 2 ) {
			tx = 2;
		}
	}
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::Sprite sprite( Drawer::Transform( 0, 0, tx * FADE_SIZE, ty * FADE_SIZE, FADE_SIZE, FADE_SIZE, WIDTH, HEIGHT ), GRAPH_FADE );
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