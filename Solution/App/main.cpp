#include "Application.h"
#include "Drawer.h"
#include "Game.h"
#include "Mouse.h"
#include "Device.h"

void main( ) {
	Application::initialize( );
	TaskPtr drawer( new Drawer( "../Resource" ) );
	TaskPtr mouse( new Mouse );
	TaskPtr game( new Game );
	TaskPtr device( new Device );
	ApplicationPtr app = Application::getInstance( );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Mouse::getTag( ), mouse );
	app->addTask( Device::getTag( ), device );
	app->addTask( Game::getTag( ), game );
}