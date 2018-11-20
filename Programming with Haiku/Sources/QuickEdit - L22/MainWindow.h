#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>

#include <Entry.h>
#include <FilePanel.h>
#include <MenuBar.h>
#include <String.h>
#include <TextView.h>

class MainWindow : public BWindow
{
public:
						MainWindow(void);
						~MainWindow(void);
			
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
			void		OpenFile(const entry_ref &ref);
			void		SaveFile(const char *path);
			
			void		FrameResized(float w, float h);
private:
			void		UpdateTextRect(void);
			
			BMenuBar	*fMenuBar;
			BTextView	*fTextView;
			BFilePanel	*fOpenPanel,
						*fSavePanel;
			
			BString		fFilePath;
};

#endif
