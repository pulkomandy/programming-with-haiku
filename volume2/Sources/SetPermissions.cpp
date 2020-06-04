#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <CheckBox.h>
#include <Font.h>
#include <Node.h>
#include <Roster.h>
#include <String.h>
#include <StringView.h>
#include <TrackerAddOn.h>
#include <Window.h>

#include <stdio.h>

#define	MSG_SET 'mset'
#define APP_SIGNATURE "application/x-vnd.dw-SetPermissions"

void
ShowError(const char *message)
{
	BAlert *alert = new BAlert("SetPerms Error", message, "OK", NULL,
								NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
	alert->Go();
}


class PMainWindow : public BWindow
{
public:
						PMainWindow(const BMessage &refMsg);

			void		MessageReceived(BMessage *msg);
			void		SetPermissions(void);
			mode_t		CalcMode(void);
			bool		IsAppMode(void);
			bool		QuitRequested(void);

private:
		BMessage		fRefMsg;
		
		BStringView		*fRLabel,
						*fWLabel,
						*fXLabel,
						*fULabel,
						*fGLabel,
						*fOLabel;
		
		BCheckBox		*fURBox,
						*fUWBox,
						*fUXBox,
						*fGRBox,
						*fGWBox,
						*fGXBox,
						*fORBox,
						*fOWBox,
						*fOXBox;
};


PMainWindow::PMainWindow(const BMessage &refMsg)
	:	BWindow(BRect(100,100,500,400), "Set File Permissions", B_TITLED_WINDOW, 
				B_WILL_ACCEPT_FIRST_CLICK | B_NOT_RESIZABLE | B_NOT_ZOOMABLE |
				B_ASYNCHRONOUS_CONTROLS | B_CLOSE_ON_ESCAPE),
		fRefMsg(refMsg)
{
	BView *top = new BView(Bounds(), "top", B_FOLLOW_ALL, B_WILL_DRAW);
	top->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(top);
	
	font_height fh;
	be_plain_font->GetHeight(&fh);
	float fontHeight = fh.ascent + fh.descent + fh.leading;
		
	// checkbox size algorithm used internally by Haiku's code.
	// Might break in future use, but it should work well enough in any event for
	// at least the near future if not longer.
	float boxWidth = ceilf(3.0f + fh.ascent);
	
	fULabel = new BStringView(BRect(0,0,1,1), "fULabel", "Owner");
	fULabel->ResizeToPreferred();
	fULabel->MoveTo(10.0, 10.0 + fontHeight + 10.0);
	top->AddChild(fULabel);
	
	fRLabel = new BStringView(BRect(0,0,1,1), "readLabel", "Execute");
	fRLabel->ResizeToPreferred();
	fRLabel->SetText("Read");
	fRLabel->SetAlignment(B_ALIGN_CENTER);
	fRLabel->MoveTo(fULabel->Frame().right + 15.0, 10.0);
	top->AddChild(fRLabel);
	
	fWLabel = new BStringView(fRLabel->Bounds(), "writeLabel", "Write");
	fWLabel->MoveTo(fRLabel->Frame().right + 10.0, 10.0);
	fWLabel->SetAlignment(B_ALIGN_CENTER);
	top->AddChild(fWLabel);
	
	fXLabel = new BStringView(fRLabel->Bounds(), "executeLabel", "Execute");
	fXLabel->SetAlignment(B_ALIGN_CENTER);
	fXLabel->MoveTo(fWLabel->Frame().right + 10.0, 10.0);
	top->AddChild(fXLabel);
	
	fURBox = new BCheckBox(BRect(0,0,15,15), "urbox", "",
									new BMessage);
	fURBox->ResizeToPreferred();
	fURBox->MoveTo(fRLabel->Frame().left + ((fRLabel->Frame().Width() - boxWidth) / 2.0),
				fULabel->Frame().top - 2.0);
	top->AddChild(fURBox);
	
	fUWBox = new BCheckBox(BRect(0,0,15,15), "uwbox", "", new BMessage);
	fUWBox->ResizeToPreferred();
	fUWBox->MoveTo(fWLabel->Frame().left + ((fWLabel->Frame().Width() - boxWidth) / 2.0),
				fURBox->Frame().top);
	top->AddChild(fUWBox);
	
	fUXBox = new BCheckBox(BRect(0,0,15,15), "uxbox", "", new BMessage);
	fUXBox->ResizeToPreferred();
	fUXBox->MoveTo(fXLabel->Frame().left + ((fXLabel->Frame().Width() - boxWidth) / 2.0),
				fURBox->Frame().top);
	top->AddChild(fUXBox);
	
	fGLabel = new BStringView(BRect(0,0,1,1), "fGLabel", "Group");
	fGLabel->ResizeToPreferred();
	fGLabel->MoveTo(10.0, fULabel->Frame().bottom + 10.0);
	top->AddChild(fGLabel);
	
	fGRBox = new BCheckBox(BRect(0,0,15,15), "grbox", "", new BMessage);
	fGRBox->ResizeToPreferred();
	fGRBox->MoveTo(fRLabel->Frame().left + ((fRLabel->Frame().Width() - boxWidth) / 2.0),
				fGLabel->Frame().top - 2.0);
	top->AddChild(fGRBox);
	
	fGWBox = new BCheckBox(BRect(0,0,15,15), "gwbox", "", new BMessage);
	fGWBox->ResizeToPreferred();
	fGWBox->MoveTo(fWLabel->Frame().left + ((fWLabel->Frame().Width() - boxWidth) / 2.0),
				fGRBox->Frame().top);
	top->AddChild(fGWBox);
	
	fGXBox = new BCheckBox(BRect(0,0,15,15), "gxbox", "", new BMessage);
	fGXBox->ResizeToPreferred();
	fGXBox->MoveTo(fXLabel->Frame().left + ((fXLabel->Frame().Width() - boxWidth) / 2.0),
				fGRBox->Frame().top);
	top->AddChild(fGXBox);
	
	
	fOLabel = new BStringView(BRect(0,0,1,1), "fOLabel", "Others");
	fOLabel->ResizeToPreferred();
	fOLabel->MoveTo(10.0, fGLabel->Frame().bottom + 10.0);
	top->AddChild(fOLabel);
	
	fORBox = new BCheckBox(BRect(0,0,15,15), "orbox", "", new BMessage);
	fORBox->ResizeToPreferred();
	fORBox->MoveTo(fRLabel->Frame().left + ((fRLabel->Frame().Width() - boxWidth) / 2.0),
				fOLabel->Frame().top - 2.0);
	top->AddChild(fORBox);
	
	fOWBox = new BCheckBox(BRect(0,0,15,15), "owbox", "", new BMessage);
	fOWBox->ResizeToPreferred();
	fOWBox->MoveTo(fWLabel->Frame().left + ((fWLabel->Frame().Width() - boxWidth) / 2.0),
				fORBox->Frame().top);
	top->AddChild(fOWBox);
	
	fOXBox = new BCheckBox(BRect(0,0,15,15), "oxbox", "", new BMessage);
	fOXBox->ResizeToPreferred();
	fOXBox->MoveTo(fXLabel->Frame().left + ((fXLabel->Frame().Width() - boxWidth) / 2.0),
				fORBox->Frame().top);
	top->AddChild(fOXBox);
	
	BButton *setButton = new BButton(BRect(0,0,1,1), "setbutton", "Set",
									new BMessage(MSG_SET), B_FOLLOW_NONE);
	setButton->ResizeToPreferred();
	setButton->MoveTo(fXLabel->Frame().right - setButton->Frame().Width(),
					fOXBox->Frame().bottom + 10.0);
	top->AddChild(setButton);
	
	ResizeTo(fXLabel->Frame().right + 10.0, setButton->Frame().bottom + 10.0);
	
	// Set some sensible defaults: 644
	fURBox->SetValue(B_CONTROL_ON);
	fGRBox->SetValue(B_CONTROL_ON);
	fORBox->SetValue(B_CONTROL_ON);
	fUWBox->SetValue(B_CONTROL_ON);
	
	fURBox->MakeFocus(true);
}


void
PMainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case MSG_SET:
		{
			SetPermissions();
			
			PostMessage(B_QUIT_REQUESTED);
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


mode_t
PMainWindow::CalcMode(void)
{
	mode_t mode = 0;
	
	if (fURBox->Value() == B_CONTROL_ON)
		mode |= S_IRUSR;
	if (fUWBox->Value() == B_CONTROL_ON)
		mode |= S_IWUSR;
	if (fUXBox->Value() == B_CONTROL_ON)
		mode |= S_IXUSR;
	
	if (fGRBox->Value() == B_CONTROL_ON)
		mode |= S_IRGRP;
	if (fGWBox->Value() == B_CONTROL_ON)
		mode |= S_IWGRP;
	if (fGXBox->Value() == B_CONTROL_ON)
		mode |= S_IXGRP;
	
	if (fORBox->Value() == B_CONTROL_ON)
		mode |= S_IROTH;
	if (fOWBox->Value() == B_CONTROL_ON)
		mode |= S_IWOTH;
	if (fOXBox->Value() == B_CONTROL_ON)
		mode |= S_IXOTH;
	
	return mode;
}


void
PMainWindow::SetPermissions(void)
{
	mode_t mode = CalcMode();
	
	int32 i = 0;
	entry_ref ref;
	while (fRefMsg.FindRef("refs", i++, &ref) == B_OK)
	{
		BEntry entry(&ref);
		if (entry.InitCheck() != B_OK || !entry.IsFile())
			continue;
		entry.Unset();
		
		BNode node(&ref);
		if (node.InitCheck() != B_OK)
			continue;
		
		node.SetPermissions(mode);
	}
}


bool
PMainWindow::IsAppMode(void)
{
	app_info ai;
	be_app->GetAppInfo(&ai);
	if (ai.signature && strcmp(ai.signature, APP_SIGNATURE) == 0)
		return true;
	return false;
}


bool
PMainWindow::QuitRequested(void)
{
	if (IsAppMode())
		be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
		

void
process_refs(entry_ref dir_ref, BMessage *msg, void *data)
{
	uint32 count = 0;
	uint32 type = 0;
	
	msg->GetInfo("refs", &type, (int32*)&count);
	if (count == 0)
	{
		ShowError("Select files/folders first!");
		return;
	}
	
	PMainWindow *win = new PMainWindow(*msg);
	win->Show();
}


class SetPermsApplication : public BApplication
{
public:
					SetPermsApplication(void);
			void	RefsReceived(BMessage *msg);
};


SetPermsApplication::SetPermsApplication(void)
	:	BApplication(APP_SIGNATURE)
{
}


void
SetPermsApplication::RefsReceived(BMessage *msg)
{
	BEntry ent("/boot");
	entry_ref ref;
	ent.GetRef(&ref);
	process_refs(ref, msg, NULL);
}


int
main(void)
{
	SetPermsApplication app;
	app.Run();
	return 0;
}

