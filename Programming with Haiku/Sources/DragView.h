#ifndef DRAGVIEW_H
#define DRAGVIEW_H

#include <View.h>

class DragView : public BView
{
public:
				DragView(BRect frame);
	void		MouseDown(BPoint pt);
	void		Draw(BRect update);

private:
	BRect		fDragRect;
};

#endif
