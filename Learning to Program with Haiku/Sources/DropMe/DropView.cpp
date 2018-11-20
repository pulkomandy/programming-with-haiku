#include "DropView.h"

// This header is needed for the definition of the entry_ref structure
#include <Entry.h>

// This is for the methods we need to work with BMessages in MessageReceived
#include <Message.h>


DropView::DropView(const BRect &frame)
	:	BView(frame, "dropview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// Once again, create a label, this time to display the file's name
	fLabel = new BStringView(BRect(10,10,11,11),"label","Drag a file here");
	fLabel->ResizeToPreferred();
	AddChild(fLabel);
}


void
DropView::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case B_SIMPLE_DATA:
		{
			// This message is the constant give to us whenever something is dropped
			// on a view from Tracker.
			
			// An entry_ref is a lightweight structure that identifies a file on disk
			// A file dropped on our BView will generate a message with the identifier
			// B_SIMPLE_DATA and attached data. The attached data takes the form of a
			// series of entry_refs with the field name "refs"
			entry_ref ref;
			
			// If FindRef is successful, the information about the file dropped on the
			// view will be copied into ref.
			if (msg->FindRef("refs",&ref) == B_OK)
			{
				// Most of the information in an entry_ref isn't very useful, but the
				// name property contains the file's name or NULL if the entry_ref is
				// empty
				fLabel->SetText(ref.name);
			}
			break;
		}
		default:
		{
			BView::MessageReceived(msg);
			break;
		}
	}
}
