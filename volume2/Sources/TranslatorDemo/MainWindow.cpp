#include "MainWindow.h"

#include <Application.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Messenger.h>
#include <ScrollView.h>
#include <stdio.h>
#include <String.h>
#include <StringView.h>
#include <TranslationKit.h>

#include "BitmapWindow.h"

enum
{
	M_SHOW_PANEL = 'shpl'
};


MainWindow::MainWindow(void)
	:	BWindow(BRect(100, 100, 500, 400), "TranslatorDemo", B_TITLED_WINDOW,
				B_ASYNCHRONOUS_CONTROLS)
{
	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r,"menubar");
	AddChild(fMenuBar);
	
	BMenu *menu = new BMenu("File");
	fMenuBar->AddItem(menu);
	
	menu->AddItem(new BMenuItem("Open", new BMessage(M_SHOW_PANEL), 'O'));
	
	BView *back = new BView(Bounds(), "background", B_FOLLOW_ALL, B_WILL_DRAW);
	back->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(back);
	
	BRect rect, textRect;
	rect = Bounds().InsetByCopy(10.0, 10.0);
	rect.top += 20;
	rect.right -= B_V_SCROLL_BAR_WIDTH;
	
	textRect = rect.OffsetToCopy(0.0, 0.0);
	textRect.InsetBy(5.0, 5.0);
	fTextView = new BTextView(rect, "textview", textRect, B_FOLLOW_ALL);
	fTextView->MakeEditable(false);
	fTextView->SetViewColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR),
									B_LIGHTEN_1_TINT));
	
	BScrollView *scrollView = new BScrollView("scrollview", fTextView,
											B_FOLLOW_ALL, 0, false, true);
	back->AddChild(scrollView);
	
	PopulateTextView();
	
	BMessenger messenger(NULL, this);
	fOpenPanel = new BFilePanel(B_OPEN_PANEL, &messenger, NULL, 0, false);
}


MainWindow::~MainWindow(void)
{
	delete fOpenPanel;
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_SHOW_PANEL:
		{
			fOpenPanel->Show();
			break;
		}
		case B_REFS_RECEIVED:
		{
			int32 i = 0;
			entry_ref ref;
			while (msg->FindRef("refs", i, &ref) == B_OK)
			{
				BitmapWindow *win = new BitmapWindow(ref);
				win->Show();
				i++;
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
MainWindow::PopulateTextView(void)
{
	BString out;
	out << 	"This app demonstrates the use of the Translation Kit. Open an "
			"image file using the File menu.\n\n"
			"Translator Information:\n-------------------------------------\n\n";
	
	// This gets a pointer to the global translator roster which already has
	// the default translators already loaded. The pointer may not be deleted.
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	
	// This call is very important if you want to find out more about the
	// Kit's capabilities. A translator_id is a unique handle for each
	// translator. Using it, we can find out specific information about each
	// one.
	int32 translatorCount;
	translator_id *translators;
	roster->GetAllTranslators(&translators, &translatorCount);
	
	for (int32 i = 0; i < translatorCount; i++)
	{
		const char *name, *info;
		int32 version;
		roster->GetTranslatorInfo(translators[i], &name, &info, &version);
		
		out << name << " version " << float(version / 100.0) << "\n\n";
		
		out << "Supported Input Formats:\n";
		
		const translation_format *inFormats;
		int32 inCount;
		roster->GetInputFormats(translators[i], &inFormats, &inCount);
		for (int32 j = 0; j < inCount; j++)
		{
			translation_format format = inFormats[j];
			out << "\tMIME type: " << format.MIME << "\n";
		}
		
		
		const translation_format *outFormats;
		int32 outCount;
		roster->GetOutputFormats(translators[i], &outFormats, &outCount);
		
		out << "\n";
	}
	
	delete [] translators;
	
	fTextView->SetText(out.String());
}


void
MainWindow::FrameResized(float w, float h)
{
	BRect textRect = fTextView->Bounds().InsetByCopy(5.0, 5.0);
	fTextView->SetTextRect(textRect);
}

