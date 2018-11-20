#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <View.h>

class MainView : public BView
{
public:
						MainView(const BRect &frame);
		void			Draw(BRect update);
};

#endif
