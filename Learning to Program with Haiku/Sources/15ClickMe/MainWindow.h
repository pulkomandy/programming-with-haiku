#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>

class MainWindow : public BWindow
{
public:
						MainWindow(void);
			
			// We are implementing the virtual BWindow method MessageReceived so that we
			// can handle the message that the button will send to the window
			void		MessageReceived(BMessage *msg);
			
private:
	// This property will hold the number of times the button has been clicked.
	int32	fCount;
};

#endif
