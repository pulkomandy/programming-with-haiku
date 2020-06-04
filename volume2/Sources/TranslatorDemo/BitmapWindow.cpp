#include "BitmapWindow.h"

#include <Bitmap.h>
#include <Screen.h>
#include <TranslationUtils.h>

#include "BitmapView.h"

BitmapWindow::BitmapWindow(const entry_ref &ref)
	:	BWindow(BRect(100, 100, 500, 400), ref.name, B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	BBitmap *image = BTranslationUtils::GetBitmap(&ref);
	fView = new BitmapView(Bounds(), "bitmapview", new BMessage, image,
							B_NO_BORDER, B_FOLLOW_ALL);
	fView->SetFixedSize(false);
	AddChild(fView);
	
	BRect screen(BScreen().Frame());
	if (image)
	{
		BRect size(image->Bounds());
		if (size.Width() > (screen.Width() * .8))
		{
			float multiplier = (screen.Width() * .8) / size.Width();
			size.right = size.Width() * multiplier;
			size.bottom = size.Height() * multiplier;
		}
		
		ResizeTo(size.Width(), size.Height());
	}
	
	BRect frame(Frame());
	MoveTo( (screen.Width() - frame.Width()) / 2.0,
			(screen.Height() - frame.Height()) / 2.0);
}

