#ifndef EDITVIEW_H
#define EDITVIEW_H

#include <Invoker.h>
#include <TextView.h>

enum
{
	M_TEXT_CHANGED = 'txch'
};

class EditView : public BTextView, public BInvoker
{
public:
							EditView(const BRect &frame, const char *name,
									const int32 &resize);
	
			void			FrameResized(float w, float h);

	virtual	void			InsertText(const char *inText, int32 inLength,
								int32 inOffset,
								const text_run_array *inRuns);
	
	virtual	void			DeleteText(int32 fromOffset, int32 toOffset);
	
			void			SetChangeNotifications(bool value);
			bool			IsNotifyingChanges(void) const;
private:
			void			UpdateTextRect(void);
			
			bool			fNotifyChanges;
};


#endif
