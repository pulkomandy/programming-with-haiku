#include "EditView.h"

EditView::EditView(const BRect &frame, const char *name, const int32 &resize)
	:	BTextView(frame, name, frame, resize),
		fNotifyChanges(true)
{
	SetFlags(Flags() | B_FRAME_EVENTS);
	UpdateTextRect();
	
	SetMessage(new BMessage(M_TEXT_CHANGED));
}

	
void
EditView::FrameResized(float w, float h)
{
	UpdateTextRect();
}


void
EditView::UpdateTextRect(void)
{
	BRect r(Bounds());
	r.InsetBy(5, 5);
	
	SetTextRect(r);
}


void
EditView::InsertText(const char *inText, int32 inLength, int32 inOffset,
					const text_run_array *inRuns)
{
	BTextView::InsertText(inText, inLength, inOffset, inRuns);
	
	if (IsNotifyingChanges())
		Invoke();
}


void
EditView::DeleteText(int32 fromOffset, int32 toOffset)
{
	BTextView::DeleteText(fromOffset, toOffset);
	
	if (IsNotifyingChanges())
		Invoke();
}


void
EditView::SetChangeNotifications(bool value)
{
	fNotifyChanges = value;
}


bool
EditView::IsNotifyingChanges(void) const
{
	return fNotifyChanges;
}

