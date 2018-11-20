//******************************************************************************
//
//	File:			SendPanel.cpp
//
//	Written by:		Douglas Wright
//
//******************************************************************************

/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include <stdio.h>
#include <Box.h>
#include <Button.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Message.h>
#include <PopUpMenu.h>
#include <ScrollView.h>
#include <TextControl.h>
#include <TextView.h>

#include "PropDump.h"
#include "SendPanel.h"

const BRect kPanelRect(120, 100, 550, 400);

const int32 SEND_MSG = 'send';
const int32 ADD_MSG	= 'addm';
const int32 REMOVE_MSG = 'remm';
const int32 POP_UP = 'popu';

const bigtime_t HUGE_TIMEOUT = 1000000;

SendPanel::SendPanel(BMessenger *msgr, BRect *frame)
	:	BWindow(kPanelRect, "Send", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	if (frame)
	{
		ResizeTo(frame->Width(), frame->Height());
		MoveTo(frame->left, frame->top);
	}
	
	SendView *view = new SendView(Bounds());
	view->SetMessenger(msgr);
	AddChild(view);
}


SendPanel::~SendPanel()
{
}



SendView::SendView(BRect frame)
	:	BView(frame, "SendView", B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLowColor(200,200,200,255);
	
	// add popup for message types
	fMessageTypeMenu = new BMenu("MessageTypeMenu");
	fMessageTypeMenu->AddItem(new BMenuItem("Get",
											new BMessage(B_GET_PROPERTY)));
	fMessageTypeMenu->AddItem(new BMenuItem("Set",
											new BMessage(B_SET_PROPERTY)));
	fMessageTypeMenu->AddItem(new BMenuItem("Create",
											new BMessage(B_CREATE_PROPERTY)));
	fMessageTypeMenu->AddItem(new BMenuItem("Delete",
											new BMessage(B_DELETE_PROPERTY)));
	fMessageTypeMenu->AddItem(new BMenuItem("Count",
											new BMessage(B_COUNT_PROPERTIES)));
	fMessageTypeMenu->AddItem(new BMenuItem("Execute",
											new BMessage(B_EXECUTE_PROPERTY)));
	fMessageTypeMenu->AddItem(new BMenuItem("Get Suites",
											new BMessage(B_GET_SUPPORTED_SUITES)));
	fMessageTypeMenu->ItemAt(0)->SetMarked(true);
	fMessageTypeMenu->SetLabelFromMarked(true);
	
	BRect rect(Bounds().InsetByCopy(10,10));
	rect.right = rect.left + 200;
	rect.bottom = rect.top + 25;
	BMenuField* menuField = new BMenuField(rect, "MenuField", "Scripting Command:",
											fMessageTypeMenu);
	menuField->SetDivider(StringWidth("Scripting Command:") + 5.0);
	AddChild(menuField);
	
	// Send button
	fSend = new BButton(BRect(0,0,1,1), "send", "Send",
									new BMessage(SEND_MSG),
									B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	AddChild(fSend);
	fSend->ResizeToPreferred();
	fSend->MoveTo(Bounds().right - 10 - fSend->Bounds().Width(), 10);
	
	// Add the BBox which holds all of the specifiers
	rect.OffsetBy(0, rect.Height() + 10);
	rect.right = Bounds().Width() - 10;
	rect.bottom = rect.top + 20.0;
	
	fSpecBox = new BBox(rect, "Box", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	fSpecBox->SetLabel("Specifiers");
	AddChild(fSpecBox);
	
	rect.OffsetBy(0, rect.Height() + 10);
	fAdd = new BButton(rect, "add", "Add", new BMessage(ADD_MSG),
						B_FOLLOW_LEFT | B_FOLLOW_TOP);
	fAdd->ResizeToPreferred();
	AddChild(fAdd);
	rect = fAdd->Frame();
	
	fRemove = new BButton(BRect(0,0,1,1), "remove", "Remove", new BMessage(REMOVE_MSG),
						B_FOLLOW_LEFT | B_FOLLOW_TOP);
	fRemove->ResizeToPreferred();
	fRemove->MoveTo(rect.right + 10, rect.top);
	fRemove->SetEnabled(false);
	AddChild(fRemove);
	
	// add Text Interface
	rect.OffsetBy(0, rect.Height() + 10.0);
	rect.right = Bounds().right - 10.0 - B_V_SCROLL_BAR_WIDTH;
	rect.bottom = Bounds().bottom - 10.0;
	
	BRect textRect(rect);
	textRect.OffsetTo(B_ORIGIN);
	textRect.left += 2;
	textRect.top += 2;
	fText = new BTextView(rect, "textview", textRect, B_FOLLOW_ALL, B_WILL_DRAW);
	fText->MakeEditable(false);
	BScrollView *scview = new BScrollView("scroller", fText, B_FOLLOW_ALL, 
         								0, false, true, B_FANCY_BORDER);
    AddChild(scview);
    
}


SendView::~SendView()
{
}


void 
SendView::AttachedToWindow()
{
	fSend->SetTarget(this);
	fAdd->SetTarget(this);
	fRemove->SetTarget(this);
	fSend->MakeDefault(true);

    // Work around the layout weirdness for BTextControl by getting its preferred size
    BTextControl *control = new BTextControl(BRect(0,0,25,25), NULL, NULL, NULL, NULL);
    control->Hide();
    Window()->AddChild(control);
    float pw;
    control->GetPreferredSize(&pw, &fTextBoxHeight);
    control->RemoveSelf();
    delete control;
}

void 
SendView::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case ADD_MSG:
		{
			AddSpecView();
			break;
		}
		case REMOVE_MSG:
		{
			RemoveSpecView();
			break;
		}
		case SEND_MSG:
		{
			DoSendMessage();
			break;
		}
		default:
		{
			BView::MessageReceived(msg);
			break;
		}
	}
}


void
SendView::FrameResized(float w, float h)
{
	BRect textRect(fText->Bounds().InsetByCopy(5,5));
	fText->SetTextRect(textRect);
}


void
SendView::DoSendMessage()
{	
	BString str;
	BMessage reply;			
	BMessage script_msg;
	BuildMessage(&script_msg);

	fText->Delete(0, fText->TextLength());

	str += "Message sent: \n";
	str += DumpMessage(&script_msg, MSG_IS_CMD);
	str += "\n\n";
	fText->Insert(fText->TextLength(), str.String(), str.Length());
	status_t result = fMessenger->SendMessage(&script_msg, &reply, HUGE_TIMEOUT, HUGE_TIMEOUT);
	if(result == B_OK)
	{
		str = "Reply received: \n";
		str += DumpMessage(&reply, MSG_IS_CMD);
		str += "\n\n";
		fText->Insert(fText->TextLength(), str.String(), str.Length());
	}
}


void 
SendView::BuildMessage(BMessage *msg) const
{
	BMenuItem *item = fMessageTypeMenu->FindMarked();
	msg->what = item->Command();
	int32 count = specViewList.CountItems();
	for(int i=count-1; i>=0; --i)
	{
		SpecView *spec = (SpecView *)specViewList.ItemAt(i);
		switch(spec->fMenu->Menu()->FindMarked()->Command())
		{
			case B_DIRECT_SPECIFIER:
			{
				msg->AddSpecifier(spec->fText1->Text());
				break;
			}
			case B_NAME_SPECIFIER:
			{
				msg->AddSpecifier(spec->fText1->Text(), spec->fText2->Text());
				break;
			}
			case B_INDEX_SPECIFIER:
			{
				int32 index;
				if (GetTCInt32(spec->fText2, &index) == B_OK)
					msg->AddSpecifier(spec->fText1->Text(), index);
				else 
				{
					printf("can't build msg: %s is not a valid index specifier!\n",
						spec->fText2->Text());
					return;
				}
				break;
			}
			case B_REVERSE_INDEX_SPECIFIER:
			{
				int32 index;
				if (GetTCInt32(spec->fText2, &index) == B_OK)
				{
					BMessage msgSpec(B_REVERSE_INDEX_SPECIFIER);
					msgSpec.AddString("property", spec->fText1->Text());
					msgSpec.AddInt32("index", index);
					msg->AddSpecifier(&msgSpec);
				}
				else
				{
					printf("can't build msg: %s is not a valid index specifier!\n",
						spec->fText2->Text());
					return;
				}
				break;
			}
			case B_RANGE_SPECIFIER:
			{
				int32 begin, end;
				if (GetTCInt32Range(spec->fText2, &begin, &end) == B_OK)
				{
					BMessage msgSpec(B_RANGE_SPECIFIER);
					msgSpec.AddString("property", spec->fText1->Text());
					msgSpec.AddInt32("index", begin);
					msgSpec.AddInt32("range", end);
					msg->AddSpecifier(&msgSpec);
				}
				else
				{
					printf("can't build msg: %s is not a valid range specifier!\n",
						spec->fText2->Text());
					return;
				}
				break;
			}
			case B_REVERSE_RANGE_SPECIFIER:
			{
				int32 begin, end;
				if (GetTCInt32Range(spec->fText2, &begin, &end) == B_OK)
				{
					BMessage msgSpec(B_REVERSE_RANGE_SPECIFIER);
					msgSpec.AddString("property", spec->fText1->Text());
					msgSpec.AddInt32("index", begin);
					msgSpec.AddInt32("range", end);
					msg->AddSpecifier(&msgSpec);
				}
				else
				{
					printf("can't build msg: %s is not a valid range specifier!\n",
						spec->fText2->Text());
					return;
				}
				break;
			}
			case B_ID_SPECIFIER:
			{
				uint32 id;
				if (GetTCUint32(spec->fText2, &id) == B_OK)
				{
					BMessage msgSpec(B_ID_SPECIFIER);
					msgSpec.AddString("property", spec->fText1->Text());
					msgSpec.AddInt32("id", id);
					msg->AddSpecifier(&msgSpec);
				}
				else
				{
					printf("can't build msg: %s is not a valid ID specifier!\n",
						spec->fText2->Text());
					return;
				}
				break;
			}						
			default:
				break;
		}
	}
}


status_t
SendView::GetTCInt32(const BTextControl* pCtrl, int32* dest) const
{
	const char* buffer = pCtrl->Text();
	char guard[2];
	int32 bufInt;
	int res = sscanf(buffer, "%ld%1s", &bufInt, guard);
	if (res != 1)
		return B_BAD_VALUE;
	else
	{
		*dest = bufInt;
		return B_OK;
	}
}


status_t
SendView::GetTCUint32(const BTextControl* pCtrl, uint32* dest) const
{
	const char* buffer = pCtrl->Text();
	const char* conversion;
	char guard[2];
	uint32 bufInt;
	if (! strncmp(buffer, "0x", 2))
		conversion = "0x%lx%1s";
	else if (! strncmp(buffer, "0X", 2))
		conversion = "0X%lx%1s";
	else if (*buffer == '0')
		conversion = "0%lo%ls";
	else
		conversion = "%lu%ls";
	
	int res = sscanf(buffer, conversion, &bufInt, guard);
	if (res != 1)
		return B_BAD_VALUE;
	else
	{
		*dest = bufInt;
		return B_OK;
	}
}


status_t
SendView::GetTCInt32Range(const BTextControl* pCtrl, int32* begin, int32* end) const
{
	const char* buffer = pCtrl->Text();
	char guard[2];
	int32 bufBegin, bufEnd;
	int res = sscanf(buffer, "%ld - %ld%1s", &bufBegin, &bufEnd, guard);
	if (res != 2)
		return B_BAD_VALUE;
	else
	{
		*begin = bufBegin;
		*end = bufEnd;
		return B_OK;
	}
}


void 
SendView::SetMessenger(BMessenger *msgr)
{
	fMessenger = msgr;
}


void 
SendView::AddSpecView()
{
	SpecView *previous = (SpecView *)specViewList.LastItem();
	
	float viewHeight = fTextBoxHeight + 5.0;
	
	Window()->ResizeBy(0, viewHeight);
	
	BView *scrollView = fText->Parent();
	scrollView->MoveBy(0, viewHeight);
	scrollView->ResizeBy(0, -viewHeight);

	fAdd->MoveBy(0, viewHeight);
	fRemove->MoveBy(0, viewHeight);
	
	fSpecBox->ResizeBy(0, viewHeight);
	
	BRect bounds;
	if (previous)
	{
		bounds = previous->Frame();
		bounds.OffsetBy(0, viewHeight);
	}
	else
	{
		bounds = fSpecBox->Bounds();
		bounds.InsetBy(5, 5);
		bounds.top += 10;
		bounds.bottom = bounds.top + viewHeight;
	}

	SpecView *specView = new SpecView(bounds);
	specViewList.AddItem(specView);
	
	fSpecBox->AddChild(specView);
	fRemove->SetEnabled(true);
}


void 
SendView::RemoveSpecView()
{
	if (specViewList.CountItems() < 1)
		return;

	SpecView *specView = (SpecView *)specViewList.LastItem();
	if (!specView)
		return;
	
	float viewHeight = fTextBoxHeight + 5.0;
	
	Window()->ResizeBy(0, -viewHeight);
	
	BView *scrollView = fText->Parent();
	scrollView->ResizeBy(0, viewHeight);
	scrollView->MoveBy(0, -viewHeight);
	
	fAdd->MoveBy(0, -viewHeight);
	fRemove->MoveBy(0, -viewHeight);
	
	fSpecBox->ResizeBy(0, -viewHeight);
	
	specViewList.RemoveItem(specView);
	specView->RemoveSelf();
	delete specView;
	
	if (specViewList.CountItems() < 1)
		fRemove->SetEnabled(false);
}


SpecView::SpecView(BRect rect)
	:	BView(rect, "SpecView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW)
{
	BRect bounds;

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLowColor(200,200,200,255);

	// add text entry boxes
	bounds = Bounds();
	bounds.right = (bounds.right / 3.0) - 2.0;
	bounds.top = 2;
	fText1 = new BTextControl(bounds, "Text1", "", "", NULL);
	fText1->SetDivider(0.0);

	BMenu *menu = new BMenu("PopUp");

	menu->SetRadioMode(true);
	menu->SetLabelFromMarked(true);
	menu->SetFont(be_plain_font);

	menu->AddItem(new BMenuItem("Direct",
								new BMessage(B_DIRECT_SPECIFIER)));
	menu->AddItem(new BMenuItem("Index",
								new BMessage(B_INDEX_SPECIFIER)));
	menu->AddItem(new BMenuItem("Name",
								new BMessage(B_NAME_SPECIFIER)));
	menu->AddItem(new BMenuItem("Reverse Index",
								new BMessage(B_REVERSE_INDEX_SPECIFIER)));
	menu->AddItem(new BMenuItem("Range",
								new BMessage(B_RANGE_SPECIFIER)));
	menu->AddItem(new BMenuItem("Reverse Range",
								new BMessage(B_REVERSE_RANGE_SPECIFIER)));
	menu->AddItem(new BMenuItem("ID",
								new BMessage(B_ID_SPECIFIER)));
	
	// mark first item
	menu->ItemAt(0)->SetMarked(true);
	
	bounds = Bounds();
	bounds.left = fText1->Frame().right + 4.0;
	fMenu = new BMenuField(bounds, "MenuField", "", menu);
	fMenu->SetDivider(0.0);
	fMenu->ResizeToPreferred();

	bounds = Bounds();
	bounds.left = fMenu->Frame().right + 4.0;
	bounds.top = 2;
	fText2 = new BTextControl(bounds, "Text2", "", "", NULL);
	fText2->SetDivider(0.0);
}


SpecView::~SpecView()
{
}


void 
SpecView::AttachedToWindow()
{
	AddChild(fText1);
	AddChild(fMenu);
	AddChild(fText2);
	
	fText1->MakeFocus();
	fMenu->Menu()->SetTargetForItems(this);
}
