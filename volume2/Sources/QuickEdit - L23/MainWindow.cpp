#include "MainWindow.h"

#include <Alert.h>
#include <Application.h>
#include <Clipboard.h>
#include <Directory.h>
#include <File.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Messenger.h>
#include <NodeInfo.h>
#include <Path.h>
#include <PrintJob.h>
#include <ScrollView.h>
#include <stdio.h>
#include <String.h>
#include <TranslationUtils.h>

#include "EditView.h"

//#define USETRACE

#ifdef USETRACE
	#define STRACE(x) printf x
#else
	#define STRACE(x) /* */
#endif


enum
{
	M_FILE_NEW = 'flnw',
	M_SHOW_OPEN = 'shop',
	M_SAVE = 'save',
	M_SAVE_AS = 'svas',
	M_PRINT_SETUP = 'ptcf',
	M_PRINT = 'prin',
	M_CUT = 'mcut',
	M_COPY = 'mcpy',
	M_PASTE = 'mpst'
};

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400), "QuickEdit", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS),
		fPageSetup(NULL),
		fChanged(false),
		fQuitAfterSave(false)
{
	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r,"menubar");
	AddChild(fMenuBar);
	
	BMenu *menu = new BMenu("File");
	fMenuBar->AddItem(menu);
	
	menu->AddItem(new BMenuItem("New", new BMessage(M_FILE_NEW), 'N'));
	
	// One item in the Haiku UI guidelines is to place an ellipsis after any menu item
	// which always shows a window
	menu->AddItem(new BMenuItem("Open" B_UTF8_ELLIPSIS, new BMessage(M_SHOW_OPEN), 'O'));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Save", new BMessage(M_SAVE), 'S'));
	menu->AddItem(new BMenuItem("Save As" B_UTF8_ELLIPSIS, new BMessage(M_SAVE_AS), 'S',
								B_COMMAND_KEY | B_SHIFT_KEY));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Page Setup" B_UTF8_ELLIPSIS, new BMessage(M_PRINT_SETUP)));
	menu->AddItem(new BMenuItem("Print" B_UTF8_ELLIPSIS, new BMessage(M_PRINT), 'P'));
	
	menu = new BMenu("Edit");
	fMenuBar->AddItem(menu);
	
	menu->AddItem(new BMenuItem("Cut", new BMessage(M_CUT), 'X'));
	menu->AddItem(new BMenuItem("Copy", new BMessage(M_COPY), 'C'));
	menu->AddItem(new BMenuItem("Paste", new BMessage(M_PASTE), 'V'));
	
	r = Bounds();
	r.top = fMenuBar->Frame().bottom;
	r.right -= B_V_SCROLL_BAR_WIDTH;
	
	BRect textRect = r;
	textRect.OffsetTo(0,0);
	textRect.InsetBy(5,5);
	fTextView = new EditView(r, "textview", B_FOLLOW_ALL);
	fTextView->SetStylable(true);
	
	BScrollView *scrollView = new BScrollView("scrollview", fTextView, B_FOLLOW_ALL, 0,
												false, true);
	AddChild(scrollView);
	fTextView->SetTarget(this);
	
	
	BMessenger msgr(NULL, this);
	fOpenPanel = new BFilePanel(B_OPEN_PANEL, &msgr, NULL, 0, false);
	fSavePanel = new BFilePanel(B_SAVE_PANEL, &msgr, NULL, 0, false);
	
	fTextView->MakeFocus(true);
}


MainWindow::~MainWindow(void)
{
	delete fOpenPanel;
	delete fSavePanel;
	delete fPageSetup;
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_FILE_NEW:
		{
			if (!AlertChanges())
				break;
			
			fTextView->SetChangeNotifications(false);
			fTextView->SetText("");
			fFilePath = "";
			fChanged = false;
			fTextView->SetChangeNotifications(true);
			break;
		}
		case M_SHOW_OPEN:
		{
			if (!AlertChanges())
				break;
			
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
		case M_TEXT_CHANGED:
		{
			fChanged = true;
			break;
		}
		case M_CUT:
		{
			fTextView->Cut(be_clipboard);
			break;
		}
		case M_COPY:
		{
			fTextView->Copy(be_clipboard);
			break;
		}
		case M_PASTE:
		{
			fTextView->Paste(be_clipboard);
			break;
		}
		case M_PRINT_SETUP:
		{
			DoPageSetup();
			break;
		}
		case M_PRINT:
		{
			DoPrint();
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
	if (!AlertChanges())
		return false;
	
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
MainWindow::OpenFile(const entry_ref &ref)
{
	BFile file(&ref, B_READ_ONLY);
	if (file.InitCheck() != B_OK)
	{
		STRACE(("Couldn't open file %s\n", ref.name));
		return;
	}
	
	fTextView->SetChangeNotifications(false);
	
	if (BTranslationUtils::GetStyledText(&file, fTextView) == B_OK)
	{
		BPath path(&ref);
		fFilePath = path.Path();
		SetTitle(path.Leaf());
		fChanged = false;
	}
	else
	{
		STRACE(("GetStyledText failed\n"));
	}

	fTextView->SetChangeNotifications(true);
}


void
MainWindow::SaveFile(const char *path)
{
	BFile file;
	if (file.SetTo(path, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE) != B_OK)
	{
		STRACE(("Couldn't create file in Save()\n"));
		return;
	}
	
	if (BTranslationUtils::PutStyledText(fTextView, &file) == B_OK)
	{
		fFilePath = path;
		fChanged = false;
		
		BNodeInfo nodeInfo(&file);
		nodeInfo.SetType("text/plain");
	}
	else
	{
		STRACE(("PutStyledText failed\n"));
	}
	
	if (fQuitAfterSave)
	{
		STRACE(("Posting quit-after-save message\n"));
		PostMessage(B_QUIT_REQUESTED);
	}
}


bool
MainWindow::AlertChanges(void)
{
	if (fChanged)
	{
		BAlert *alert = new BAlert("QuickEdit",
									"The file has changed. Do you want to save them?",
									"Don't Save", "Cancel", "Save");
		int32 result = alert->Go();
		if (result == 1)
		{
			STRACE(("AlertChanges: returning false\n"));
			return false;
		}
		
		if (result == 2)
		{
			if (fFilePath.CountChars() < 1)
			{
				fQuitAfterSave = true;
				PostMessage(M_SAVE);
				STRACE(("AlertChanges: returning false\n"));
				return false;
			}
		}
	}
	
	STRACE(("AlertChanges: returning true\n"));
	return true;
}


void
MainWindow::DoPageSetup(void)
{
	// Support for page setup is dead easy because the print server does almost
	// all of the work for us. All that we need to do is supply a name for the 
	// job, call the ConfigPage() method, and if everything went well, save the
	// print settings set by the user by calling Settings();
	BString docName;
	if (fFilePath.CountChars() > 0)
	{
		BPath path(fFilePath);
		docName = path.Leaf();
	}
	else
		docName = "Untitled";
		
	BPrintJob job(docName);
	if (fPageSetup)
		job.SetSettings(fPageSetup);
	
	if (job.ConfigPage() == B_OK)
	{
		// Settings() returns a new BMessage which is owned by the caller and
		// not the BPrintJob object.
		delete fPageSetup;
		fPageSetup = job.Settings();
	}
}


void
MainWindow::DoPrint(void)
{
	BString docName;
	if (fFilePath.CountChars() > 0)
	{
		BPath path(fFilePath);
		docName = path.Leaf();
	}
	else
		docName = "Untitled";
	
	// The following portion of code was adapted from MainWindow.cpp in
	// Haiku's StyledEdit application.
	
 	// Copyright 2002-2010, Haiku, Inc. All Rights Reserved.
	// Distributed under the terms of the MIT License.
	
	BPrintJob job(docName.String());
	if (fPageSetup)
	{
		// We need to make a copy of our settings message if it exists because
		// SetSettings() takes ownership of the message passed to it.
		BMessage *settingsCopy = new BMessage(*fPageSetup);
		job.SetSettings(settingsCopy);
	}
	
	// Calling ConfigJob() shows the print dialog and confirms the print
	// settings. It can also set up the page setup if it has not already been
	// set.
	if (job.ConfigJob() != B_OK)
 		return;
	
	delete fPageSetup;
	fPageSetup = job.Settings();
	
	BRect printableRect = job.PrintableRect();
	int32 firstPage = job.FirstPage();
	int32 lastPage = job.LastPage();

	// We'll use these two variables to calculate print layout
	int32 firstLine = 0;
	int32 lastLine = fTextView->CountLines();

	int32 pagesInDocument = 1;
	int32 linesInDocument = fTextView->CountLines();
	
	// The layout calculation loop which determines how many pages will need printed,
	// We have to figure the layout for the entire document before we can do any
	// printing because we may not be starting from page 1.
	int32 currentLine = 0;
	while (currentLine < linesInDocument)
	{
		float currentHeight = 0;
		while (currentHeight < printableRect.Height() && currentLine < linesInDocument)
		{
			currentHeight += fTextView->LineHeight(currentLine);
			if (currentHeight < printableRect.Height())
				currentLine++;
		}
		
		if (pagesInDocument == lastPage)
			lastLine = currentLine - 1;
		
		if (currentHeight >= printableRect.Height())
		{
			pagesInDocument++;
			if (pagesInDocument == firstPage)
				firstLine = currentLine;
		}
	}

	if (lastPage > pagesInDocument - 1)
	{
		lastPage = pagesInDocument - 1;
		lastLine = currentLine - 1;
	}
	
	job.BeginJob();
	if (fTextView->CountLines() > 0 && fTextView->TextLength() > 0)
	{
		int32 printLine = firstLine;
		while (printLine <= lastLine)
		{
			// Here we calculate the size of the text rectangle for when
			// we tell the view to "draw" itself to the print job
			float currentHeight = 0;
			int32 firstLineOnPage = printLine;
			while (currentHeight < printableRect.Height() && printLine <= lastLine)
			{
				currentHeight += fTextView->LineHeight(printLine);
				if (currentHeight < printableRect.Height())
					printLine++;
			}
			
			float top = 0;
			if (firstLineOnPage != 0)
				top = fTextView->TextHeight(0, firstLineOnPage - 1);
			
			float bottom = fTextView->TextHeight(0, printLine - 1);
			BRect textRect(0.0, top + 5.0, printableRect.Width(), bottom + 5.0);
			
			// These calls send the draw instructions to the job object...
			job.DrawView(fTextView, textRect, B_ORIGIN);
			
			// ...and then saves it to disk. This must be called once per page
			job.SpoolPage();
		}
	}
	job.CommitJob();
}

