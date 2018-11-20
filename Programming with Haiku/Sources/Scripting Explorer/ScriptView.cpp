/*
	ScriptView.cpp
	
	Copyright 1997 Be Incorporated, All Rights Reserved.
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include "ScriptView.h"
#include <Roster.h>
#include <Screen.h>
#include <stdio.h>
#include <string.h>
#include <String.h>

#include "PropDump.h"
#include "SendPanel.h"

const bigtime_t HUGE_TIMEOUT = 1000000;

ScriptView::ScriptView(BRect rect, const char *name)
	: BOutlineListView(rect, name, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL)
{
	BMessage *msg = new BMessage ('invk');
	SetInvocationMessage(msg);
}


status_t
ScriptView::FillList()
{
	BString name;
	int32 wincount, viewcount;
	BMessage reply;
	BList teams;
	status_t result;
	
	be_roster->GetAppList(&teams);
	int32 n = teams.CountItems();
	for(int32 i=0; i<n; i++)
	{
		team_id id = (team_id)teams.ItemAt(i);
		BMessenger msgr(NULL, id);
		
		// ask for App Name
		BMessage name_msg(B_GET_PROPERTY);
		name_msg.AddSpecifier("Name");
		if (msgr.SendMessage(&name_msg, &reply, HUGE_TIMEOUT, HUGE_TIMEOUT) != B_OK)
			continue;
		
		if (reply.FindString("result", 0, &name) != B_OK)
			continue;
		
		if (name.IFindLast("_daemon") >= 0 || name.IFindLast("_server") >= 0 ||
			name == "registrar")
			continue;
		
		BStringItem *app = new BStringItem(name.String(), 0, false);
		AddItem(app);
		
		// ask for Window Count
		BMessage count_msg(B_COUNT_PROPERTIES);
		count_msg.AddSpecifier("Window");
		if (msgr.SendMessage(&count_msg, &reply, HUGE_TIMEOUT, HUGE_TIMEOUT) != B_OK)
			continue;
		
		if (reply.FindInt32("result", 0, &wincount) != B_OK)
			continue;
		
		for(int i=0; i < wincount; i++)
		{
			BMessage winname_msg(B_GET_PROPERTY);
			winname_msg.AddSpecifier("Title");
			winname_msg.AddSpecifier("Window", i);
			if (msgr.SendMessage(&winname_msg, &reply, HUGE_TIMEOUT, HUGE_TIMEOUT) != B_OK)
				continue;
			
			result = reply.FindString("result", 0, &name);
			if(result == B_OK)
			{
				BStringItem *winname = new BStringItem(name.String(), 1, false);
				AddItem(winname);
			}
			
			// count Views
			BMessage view_msg(B_COUNT_PROPERTIES);
			view_msg.AddSpecifier("View");
			view_msg.AddSpecifier("Window", i);
			if (msgr.SendMessage(&view_msg, &reply, HUGE_TIMEOUT, HUGE_TIMEOUT) != B_OK)
				continue;
				
			if (reply.FindInt32("result", 0, &viewcount) != B_OK)
				continue;
			
			// ask for View Names
			for(int j=0; j < viewcount; j++)
			{
				BMessage viewname_msg(B_GET_PROPERTY);
				viewname_msg.AddSpecifier("InternalName");
				viewname_msg.AddSpecifier("View", j);
				viewname_msg.AddSpecifier("Window", i);
				
				if (msgr.SendMessage(&viewname_msg, &reply, HUGE_TIMEOUT, HUGE_TIMEOUT) != B_OK)
					break;
				
				// reply.PrintToStream();
				
				if(reply.FindString("result", 0, &name) != B_OK)
					break;
				
				BStringItem *viewname = new BStringItem(name.String(), 2, false);
				AddItem(viewname);								
			}
		}
	}
	return B_OK;
}


status_t
ScriptView::Invoke(BMessage *msg)
{
	const char *name;
	BMessenger *sendto = new BMessenger;
	BMessage script_msg(B_GET_PROPERTY);
	BStringItem *temp;
	BList teams;
	BMessage reply;
	status_t result;
	
	script_msg.AddSpecifier("Messenger");
	
	int32 index = CurrentSelection();
	BStringItem *item = (BStringItem *)ItemAt(index);
	if(item->OutlineLevel() == 2)
	{
		script_msg.AddSpecifier("View", item->Text());
		temp = (BStringItem *)Superitem(item);
		item = temp;
	}
	if(item->OutlineLevel() == 1)
	{
		script_msg.AddSpecifier("Window", item->Text());
		temp = (BStringItem *)Superitem(item);
		item = temp;
	}
	if(item->OutlineLevel() == 0)
	{
		be_roster->GetAppList(&teams);
		int n = teams.CountItems();
		for(int i=0; i<n; i++){
			team_id id = (team_id)teams.ItemAt(i);
			BMessenger msgr(NULL, id);
			// ask for App Name
			BMessage name_msg(B_GET_PROPERTY);
			name_msg.AddSpecifier("Name");
			result = msgr.SendMessage(&name_msg, &reply, HUGE_TIMEOUT, HUGE_TIMEOUT);
			if(result == B_OK)
			{
				reply.PrintToStream();
				result = reply.FindString("result", 0, &name);
				if(result == B_OK){
					if( strcmp(name, item->Text()) < 1 )
					{
						// get a messenger for this object
						printf("string compare\n");
						script_msg.PrintToStream();
						result = msgr.SendMessage(&script_msg, &reply, HUGE_TIMEOUT, HUGE_TIMEOUT);
						if (result == B_OK)
						{
							reply.PrintToStream();
							result = reply.FindMessenger("result", sendto);
							if(result == B_OK)
							{
								BRect frame(Window()->Frame());
								BRect screen(BScreen().Frame());
								
								frame.OffsetTo(frame.right + 10.0, frame.top);
								frame.right = frame.left + 430;
								frame.bottom = frame.top + 300;
								if (frame.right > screen.right)
									frame.OffsetBy(screen.right - frame.right - 10, 0);
									
								SendPanel *panel = new SendPanel(sendto, &frame);
								panel->Show();
								return B_OK;
							}
						}
					}
				}
			}
		}
	}
	return BOutlineListView::Invoke(msg);
}



