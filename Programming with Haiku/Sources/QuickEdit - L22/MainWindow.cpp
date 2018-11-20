#include "MainWindow.h"

#include <Alert.h>
#include <Application.h>
#include <Directory.h>
#include <File.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Messenger.h>
#include <NodeInfo.h>
#include <Path.h>
#include <ScrollView.h>
#include <String.h>
#include <TranslationUtils.h>

enum
{
	M_FILE_NEW = 'flnw',
	M_SHOW_OPEN = 'shop',
	M_SAVE = 'save',
	M_SAVE_AS = 'svas',
	M_PRINT_SETUP = 'ptcf',
	M_PRINT = 'prin'
};

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400), "QuickEdit", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	// We will first start with creating the menu bar and populating the menu with
	// commands that we will be implementing.
	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r,"menubar");
	AddChild(fMenuBar);
	
	BMenu *menu = new BMenu("File");
	fMenuBar->AddItem(menu);
	
	// This is a shorthand way of adding new items to a menu. Note that the key
	// accelerators are standards for Haiku:
	//		Alt + N => New file
	//		Alt + O => Open file
	//		Alt + S => Save file
	//		Alt + Shift + S => Save As
	menu->AddItem(new BMenuItem("New", new BMessage(M_FILE_NEW), 'N'));
	menu->AddItem(new BMenuItem("Open", new BMessage(M_SHOW_OPEN), 'O'));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Save", new BMessage(M_SAVE), 'S'));
	
	// This version, unlike the others above, specifies a key accelerator which uses
	// more than just Alt plus a letter.
	menu->AddItem(new BMenuItem("Save As" B_UTF8_ELLIPSIS, new BMessage(M_SAVE_AS),
								'S', B_COMMAND_KEY | B_SHIFT_KEY));
	
	// Now we will add the text view and scrollbars. To do this we will use a
	// BScrollView for convenience. When working with a BScrollView, you create
	// its target first, create the BScrollView second, and then call AddChild
	// only for the BScrollView -- it will take care of adding its target for you.
	r = Bounds();
	r.top = fMenuBar->Frame().bottom + 1;
	
	// When calculating the size of the target view, you must compensate for the extra
	// width and/or height of the scrollbars provided by BScrollView. Haiku gives us
	// the constants B_V_SCROLL_BAR_WIDTH and B_H_SCROLL_BAR_HEIGHT for this.
	r.right -= B_V_SCROLL_BAR_WIDTH;
	
	// The BTextView constructor we will be using expects a frame size but it also
	// requires a rectangle for the area where the text will appear. This more or less
	// sets the margins.
	BRect textRect = r;
	textRect.OffsetTo(0,0);
	textRect.InsetBy(5,5);
	fTextView = new BTextView(r, "textview", textRect, B_FOLLOW_ALL);
	
	// Without this call, our BTextView will only be a plain text editor
	fTextView->SetStylable(true);
	
	
	BScrollView *scrollView = new BScrollView("scrollview", fTextView, B_FOLLOW_ALL, 0,
												false, true);
	AddChild(scrollView);
	
	// Here we will use a new class: BFilePanel. More on this later.
	BMessenger msgr(NULL, this);
	fOpenPanel = new BFilePanel(B_OPEN_PANEL, &msgr, NULL, 0, false);
	fSavePanel = new BFilePanel(B_SAVE_PANEL, &msgr, NULL, 0, false);
	
	// This will make it possible for the user to start typing as soon as the program
	// is running. Not doing so is a major annoyance for the user.
	fTextView->MakeFocus(true);
}


MainWindow::~MainWindow(void)
{
	delete fOpenPanel;
	delete fSavePanel;
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_FILE_NEW:
		{
			// Erase all text in the BTextView and reset the file path because
			// any data we will have in it will not have been saved to disk.
			fTextView->SetText("");
			fFilePath = "";
			break;
		}
		
		// These cases handle messages related to opening and saving files.
		case M_SHOW_OPEN:
		{
			fOpenPanel->Show();
			break;
		}
		case B_REFS_RECEIVED:
		{
			entry_ref ref;
			if (msg->FindRef("refs", &ref) != B_OK)
				break;
			OpenFile(ref);
			break;
		}
		case M_SAVE:
		{
			if (fFilePath.CountChars() < 1)
				fSavePanel->Show();
			else
				SaveFile(fFilePath.String());
			break;
		}
		case M_SAVE_AS:
		{
			fSavePanel->Show();
			break;
		}
		case B_SAVE_REQUESTED:
		{
			entry_ref dir;
			BString name;
			if (msg->FindRef("directory", &dir) == B_OK &&
				msg->FindString("name", &name) == B_OK)
			{
				BPath path(&dir);
				path.Append(name);
				SaveFile(path.Path());
			}
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
MainWindow::OpenFile(const entry_ref &ref)
{
	// The Translation Kit provides translation services for text files, too.
	BFile file(&ref, B_READ_ONLY);
	if (file.InitCheck() != B_OK)
		return;
	
	// One simple function to read styled text from a file. Nice!
	if (BTranslationUtils::GetStyledText(&file, fTextView) == B_OK)
	{
		// BPath gives us a bridge between the data classes used by the
		// Storage Kit and a regular string-based path. Here we set the BPath
		// instance to the full path of the file we have opened and we will
		// set the window's title to the file's name.
		BPath path(&ref);
		fFilePath = path.Path();
		SetTitle(path.Leaf());
	}
}


void
MainWindow::SaveFile(const char *path)
{
	// This function takes a string path and saves the data in the BTextView
	// to the file, which is either created or overwrites an existing one.
	BFile file;
	if (file.SetTo(path, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE) != B_OK)
		return;
	
	if (BTranslationUtils::PutStyledText(fTextView, &file) == B_OK)
	{
		fFilePath = path;
		
		BNodeInfo nodeInfo(&file);
		nodeInfo.SetType("text/plain");
	}
}


void
MainWindow::FrameResized(float w, float h)
{
	// Here is one of those little nuggets of wisdom to remember: when a BTextView
	// is resized, it does NOT update its text rectangle. When the window is resized,
	// so is our BTextView. The FrameResized() method ensures that the text rectangle
	// is updated.
	UpdateTextRect();
}


void
MainWindow::UpdateTextRect(void)
{
	BRect r(fTextView->Bounds());
	r.InsetBy(5, 5);
	fTextView->SetTextRect(r);
}

