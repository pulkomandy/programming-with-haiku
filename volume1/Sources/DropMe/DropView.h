#ifndef DROPVIEW_H
#define DROPVIEW_H

#include <View.h>
#include <StringView.h>

class DropView : public BView
{
public:
			DropView(const BRect &frame);
	void	MessageReceived(BMessage *msg);

private:
	BStringView	*fLabel;
};


#endif
