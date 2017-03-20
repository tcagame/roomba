#include "Application.h"
#include "Drawer.h"
#include "Game.h"

void main( ) {
	Application::initialize( );
	TaskPtr drawer( new Drawer( "../Resource" ) );
	TaskPtr game( new Game );
	ApplicationPtr app = Application::getInstance( );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Game::getTag( ), game );
}