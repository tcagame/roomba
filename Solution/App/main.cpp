#include "Application.h"
#include "Drawer.h"
#include "Game.h"
#include "Mouse.h"
#include "Device.h"
#include "Keyboard.h"
#include "Sound.h"

void main( ) {
	Application::initialize( );
	TaskPtr drawer( new Drawer( "../Resource" ) );
	TaskPtr sound( new Sound( "../Sound" ) );
	TaskPtr mouse( new Mouse );
	TaskPtr game( new Game );
	TaskPtr device( new Device );
	TaskPtr keyboard( new Keyboard );
	ApplicationPtr app = Application::getInstance( );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Sound::getTag( ), sound );
	app->addTask( Mouse::getTag( ), mouse );
	app->addTask( Device::getTag( ), device );
	app->addTask( Keyboard::getTag( ), keyboard );
	app->addTask( Game::getTag( ), game );
}