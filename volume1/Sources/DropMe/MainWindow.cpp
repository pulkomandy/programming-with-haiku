#include "MainWindow.h"

#include <Message.h>

#include "DropView.h"

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,400,300),"DropMe",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | 
																B_QUIT_ON_WINDOW_CLOSE)
{
	DropView *view = new DropView(Bounds());
	AddChild(view);
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

