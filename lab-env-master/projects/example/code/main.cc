//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"

int
main()
{
	Example::Application app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
	
}