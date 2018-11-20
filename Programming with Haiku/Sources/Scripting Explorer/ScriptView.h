/*
	ScriptView.h
	
	Copyright 1997 Be Incorporated, All Rights Reserved.
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef SCRIPT_VIEW_H
#define SCRIPT_VIEW_H

#include <OutlineListView.h>

class ScriptView : public BOutlineListView
{
public:
						ScriptView(BRect frame, const char *name);
	
	virtual status_t	FillList();
	virtual status_t	Invoke(BMessage *msg = NULL);
};

#endif //SCRIPT_VIEW_H
