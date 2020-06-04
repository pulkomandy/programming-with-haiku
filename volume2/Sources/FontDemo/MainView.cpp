#include "MainView.h"

MainView::MainView(const BRect &frame)
	:	BView(frame, "textview", B_FOLLOW_ALL, B_WILL_DRAW)
{
}


void
MainView::Draw(BRect update)
{
	// The Draw hook function is called whenever a BView is asked to redraw
	// itself on the screen. We will just write some code to draw some text.
	BFont font;
	font.SetSize(18.0);
	
	// Just setting values to our font object isn't enough. We have to assign
	// the BView's font settings to the values kept in our font object.
	SetFont(&font);
	
	// DrawString uses the BView's current font settings, draw mode, and
	// color to draw the text. Note that the point specified is the left
	// end of the baseline, so calculating where to draw text can be a
	// little backwards when compared to drawing anything else.
	DrawString("This is some text", BPoint(50,50));
}
