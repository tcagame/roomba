#include "Application.h"
#include "Drawer.h"
#include "Game.h"
#include "Mouse.h"
#include "Keyboard.h"

void main( ) {
	Application::initialize( );
	TaskPtr drawer( new Drawer( "../Resource" ) );
	TaskPtr mouse( new Mouse );
	TaskPtr keyboard( new Keyboard );
	TaskPtr game( new Game );
	ApplicationPtr app = Application::getInstance( );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Mouse::getTag( ), mouse );
	app->addTask( Keyboard::getTag( ), keyboard );
	app->addTask( Game::getTag( ), game );
}