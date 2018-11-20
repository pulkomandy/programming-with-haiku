/*
	BitmapView.cpp: A simple picture display control with some nice extras
	Written by DarkWyrm <darkwyrm@earthlink.net>, Copyright 2007
	Released under the MIT license.
*/
#ifndef BMPVIEW_H
#define BMPVIEW_H

#include <Message.h>
#include <Bitmap.h>
#include <Invoker.h>
#include <String.h>
#include <View.h>
#include <PopUpMenu.h>

enum
{
	M_BITMAP_ADDED='mbma',
	M_BITMAP_REMOVED='mbmr'
};

class BitmapView : public BView, public BInvoker
{
public:
							BitmapView(const BRect &frame, const char *name,
										BMessage *mod, BBitmap *bitmap, 
										border_style = B_PLAIN_BORDER,
										const int32 &resize = B_FOLLOW_LEFT | 
																B_FOLLOW_TOP,
										const int32 &flags = B_WILL_DRAW);
							~BitmapView(void);
						
	virtual	void			AttachedToWindow(void);
	virtual	void			Draw(BRect rect);
	virtual	void			MessageReceived(BMessage *msg);
	virtual	void			MouseDown(BPoint pt);
	
	virtual	void			SetBitmap(BBitmap *bitmap);
			BBitmap *		GetBitmap(void) const;
	
	virtual	void			SetEnabled(bool value);
			bool			IsEnabled(void) const;
	
	virtual void			SetStyle(border_style style);
			border_style	Style(void) const;
	
			void			SetFixedSize(bool isfixed);
			bool			IsFixedSize(void) const;
	
	virtual	void			SetAcceptDrops(bool accept);
			bool			AcceptsDrops(void) const;
	
	virtual	void			SetAcceptPaste(bool accept);
			bool			AcceptsPaste(void) const;
	
	virtual	void			SetConstrainDrops(bool value);
			bool			ConstrainsDrops(void) const;
	
	virtual	void			SetMaxBitmapSize(const float &width, const float &height);
			void			MaxBitmapSize(float *width, float *height) const;
	
			void			SetBitmapRemovable(bool isremovable);
			bool			IsBitmapRemovable(void) const;
	
			void			RemoveBitmap(void);
			void			PasteBitmap(void);
	
	virtual	void			FrameResized(float width, float height);
	
private:
			void			CalculateBitmapRect(void);
			BRect			ScaleRectToFit(const BRect &from, const BRect &to);
			void			ConstrainBitmap(void);
			
			bool			ClipboardHasBitmap(void);
			BBitmap *		BitmapFromClipboard(void);
	
	float 			fNoPhotoWidths[4];
	BPoint			fNoPhotoOffsets[4];
	
	BBitmap			*fBitmap;
	BRect			fBitmapRect;
	bool			fEnabled;
	border_style	fBorderStyle;
	
	bool			fFixedSize,
					fAcceptDrops,
					fAcceptPaste,
					fConstrainDrops;
	
	float			fMaxWidth,
					fMaxHeight;
	
	bool			fRemovableBitmap;
	BPopUpMenu 		*fPopUpMenu;
	uint32			fMouseButtons;
	
	BMenuItem		*fPasteItem;
	BMenuItem		*fRemoveItem;
};

#endif
