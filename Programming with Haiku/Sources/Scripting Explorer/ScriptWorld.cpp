/*
	ScriptWorld.cpp
	
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include <Application.h>

#include "ScriptWindow.h"

class ScriptApplication : public BApplication 
{
public:
	ScriptApplication();
};


ScriptApplication::ScriptApplication()
	:	BApplication("application/x-vnd.dw-ScriptingExplorer")
{
	ScriptWindow *aWindow = new ScriptWindow(BRect(100, 80, 300, 280));
	aWindow->Show();
}


int
main(void)
{	
	ScriptApplication().Run();
	return 0;
}


