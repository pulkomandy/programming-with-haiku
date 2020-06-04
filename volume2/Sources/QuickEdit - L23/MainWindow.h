#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>

#include <Entry.h>
#include <FilePanel.h>
#include <MenuBar.h>
#include <String.h>

class EditView;

class MainWindow : public BWindow
{
public:
						MainWindow(void);
						~MainWindow(void);
			
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
			void		OpenFile(const entry_ref &ref);
			void		SaveFile(const char *path);
			
private:
			bool		AlertChanges(void);
			void		DoPageSetup(void);
			void		DoPrint(void);
			
			BMenuBar	*fMenuBar;
			EditView	*fTextView;
			BFilePanel	*fOpenPanel,
						*fSavePanel;
			
			BMessage	*fPageSetup;
			
			BString		fFilePath;
			bool		fChanged;
			bool		fQuitAfterSave;
};

#endif
