#include "Application.h"
#include "Drawer.h"
#include "Editor.h"
#include "Mouse.h"
#include "Keyboard.h"

void main( ) {
	Application::initialize( );
	TaskPtr drawer( new Drawer( "../Resource" ) );
	TaskPtr mouse( new Mouse );
	TaskPtr keyboard( new Keyboard );
	TaskPtr editor( new Editor );
	ApplicationPtr app = Application::getInstance( );
	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Mouse::getTag( ), mouse );
	app->addTask( Keyboard::getTag( ), keyboard );
	app->addTask( Editor::getTag( ), editor );
}