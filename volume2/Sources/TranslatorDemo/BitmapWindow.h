#ifndef BITMAPWINDOW_H
#define BITMAPWINDOW_H

#include <Entry.h>
#include <Window.h>

class BitmapView;

class BitmapWindow : public BWindow
{
public:
					BitmapWindow(const entry_ref &ref);

private:
	 BitmapView		*fView;
};

#endif
