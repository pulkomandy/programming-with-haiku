#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <FilePanel.h>
#include <MenuBar.h>
#include <TextView.h>

class MainWindow : public BWindow
{
public:
						MainWindow(void);
						~MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			void		FrameResized(float w, float h);
			
private:
			void		PopulateTextView(void);
			
			BMenuBar	*fMenuBar;
			BTextView	*fTextView;
			
			BFilePanel	*fOpenPanel;
};

#endif
