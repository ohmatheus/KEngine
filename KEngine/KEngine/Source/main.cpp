#include "stdafx.h"
#include "KApplication.h"

#include <GLFW/glfw3.h>

//----------------------------------------------------------

int main(int argc, char *argv[])
{
	KApplication *app = new KApplication();
	app->Setup();
	app->Loop();
	app->Terminate();

	delete app;
	return 1;
}

//----------------------------------------------------------
