#include "DragView.h"

enum
{
	M_DRAG = 'drag'
};


DragView::DragView(BRect frame)
	:	BView(frame, "dragview", B_FOLLOW_LEFT | B_FOLLOW_TOP,
			B_WILL_DRAW),
		fDragRect(10, 10, 50, 50)
{
}


void
DragView::MouseDown(BPoint pt)
{
	// For our example, we only support dragging with the left button
	BPoint temp;
	uint32 buttons;
	GetMouse(&temp, &buttons);
	
	if (fDragRect.Contains(temp))
	{
		// SetMouseEventMask can only be called from within
		// MouseDown(). It forces all mouse events to be sent to this
		// view until the button is released. This saves us from having
		// to manually code the mouse tracking.
		SetMouseEventMask(B_POINTER_EVENTS, 0);
		
		BMessage dragMsg(M_DRAG);
		dragMsg.AddInt32("buttons", buttons);
		DragMessage(&dragMsg, fDragRect);
		
		// If you allocate the drag message on the heap, make sure
		// you delete it after DragMessage() returns. If you use a
		// BBitmap version of DrawMessage(), DON'T delete the bitmap
		// after calling DragMessage() because the system will do
		// that for you.
	}
	
}


void
DragView::Draw(BRect update)
{
	SetHighColor(0, 0, 160);
	FillRect(fDragRect);
	
	DrawString("Try dragging the square", BPoint(fDragRect.left,
								fDragRect.bottom + 50));
}

