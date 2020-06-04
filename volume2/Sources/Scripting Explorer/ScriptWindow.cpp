/*
	ScriptWindow.cpp
	
	Copyright 1995 Be Incorporated, All Rights Reserved.
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include <Application.h>
#include <ScrollView.h>
#include "ScriptWindow.h"
#include "ScriptView.h"

ScriptWindow::ScriptWindow(BRect frame)
	:	BWindow(frame, "Scripting Explorer", B_TITLED_WINDOW, B_NOT_ZOOMABLE |
													B_QUIT_ON_WINDOW_CLOSE)
{
	ScriptView	*scriptView;
	
	BRect r(Bounds());
	
	r.right -= B_V_SCROLL_BAR_WIDTH;
	scriptView = new ScriptView(r, "ScriptView");
	
	BScrollView *scrollView = new BScrollView("scrollView", scriptView,
											B_FOLLOW_ALL, 0, false, true);
	AddChild(scrollView);
	
	scriptView->FillList();
	
	// Items in the list will all have the same height, so we can assume that
	// item height * item count is OK.
	BListItem *item = scriptView->ItemAt(0L);
	if (item)
	{
		BRect itemFrame = scriptView->ItemFrame(0L);
		int32 count = scriptView->CountItems();
		count = MAX(count, 15);
		ResizeTo(Bounds().IntegerWidth(),itemFrame.IntegerHeight() * count);
	}
}
