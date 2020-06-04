#include "ColorWell - L19.h"
#include <PropertyInfo.h>
#include <string.h>

static property_info sColorWellProperties[] =
{
	{	"IsRound", { B_GET_PROPERTY, B_SET_PROPERTY, 0 },
		{ B_DIRECT_SPECIFIER, 0 },
		"True if the color well is round, false if rectangular.", 0,
		{ B_BOOL_TYPE }
	},
	{	"Red", { B_GET_PROPERTY, B_SET_PROPERTY, 0 },
		{ B_DIRECT_SPECIFIER, 0 },
		"The red value for the color well.", 0,
		{ B_INT32_TYPE }
	},
	{	"Green", { B_GET_PROPERTY, B_SET_PROPERTY, 0 },
		{ B_DIRECT_SPECIFIER, 0 },
		"The green value for the color well.", 0,
		{ B_INT32_TYPE }
	},
	{	"Blue", { B_GET_PROPERTY, B_SET_PROPERTY, 0 },
		{ B_DIRECT_SPECIFIER, 0 },
		"The blue value for the color well.", 0,
		{ B_INT32_TYPE }
	},
};


ColorWell::ColorWell(BRect frame, const char *name, BMessage *message,
	int32 resize, int32 flags, int32 style)
	: BControl(frame,name,NULL,message, resize, flags | B_SUBPIXEL_PRECISE)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLowColor(0,0,0);
	
	fColor.red = 0;
	fColor.green = 0;
	fColor.blue = 0;
	fColor.alpha = 255;
	
	fDisabledColor.red = 190;
	fDisabledColor.green = 190;
	fDisabledColor.blue = 190;
	fDisabledColor.alpha = 255;
}


ColorWell::ColorWell(BMessage *data)
	:	BControl(data)
{
	// This will update fColor
	SetValue(Value());
}


ColorWell::~ColorWell(void)
{
}


status_t
ColorWell::GetSupportedSuites(BMessage *msg)
{
	msg->AddString("suites","suite/vnd.dw-colorwell");
	BPropertyInfo prop_info(sColorWellProperties);
	msg->AddFlat("messages",&prop_info);
	
	return BControl::GetSupportedSuites(msg);
}


BHandler *
ColorWell::ResolveSpecifier(BMessage *msg, int32 index, BMessage *specifier,
							int32 what, const char *property)
{
	BPropertyInfo propertyInfo(sColorWellProperties);
	if (propertyInfo.FindMatch(msg, index, specifier, what, property) >= 0)
		return this;
	
	return BControl::ResolveSpecifier(msg, index, specifier, what, property);
}


void
ColorWell::SetValue(int32 value)
{
	BControl::SetValue(value);
	
	fColor.red = (value & 0xFF000000) >> 24;
	fColor.green = (value & 0x00FF0000) >> 16;
	fColor.blue = (value & 0x0000FF00) >> 8;
	fColor.alpha = 255;
	
	SetHighColor(fColor);
	Draw(Bounds());
	Invalidate();
}


void
ColorWell::SetValue(const rgb_color &col)
{
	fColor = col;
	fColor.alpha = 255;
	
	SetHighColor(col);
	Draw(Bounds());
	Invalidate();
}


void
ColorWell::SetValue(const uint8 &r,const uint8 &g, const uint8 &b)
{
	fColor.red = r;
	fColor.green = g;
	fColor.blue = b;
	fColor.alpha = 255;
	
	SetHighColor(r,g,b);
	Draw(Bounds());
	Invoke();
}


void
ColorWell::SetStyle(const int32 &style)
{
	if (style != fStyle) {
		fStyle = style;
		Invalidate();
	}
}


int32
ColorWell::Style(void) const
{
	return fStyle;
}


void
ColorWell::MessageReceived(BMessage *msg)
{
	if (!msg->HasSpecifiers())
		BControl::MessageReceived(msg);
	
	BMessage reply(B_REPLY);
	status_t status = B_ERROR;
	int32 index;
	BMessage specifier;
	int32 what;
	const char *property;
	
	if (msg->GetCurrentSpecifier(&index, &specifier, &what, &property) != B_OK)
		return BHandler::MessageReceived(msg);

	BPropertyInfo propInfo(sColorWellProperties);
	switch (propInfo.FindMatch(msg, index, &specifier, what, property))
	{
		case 0:	// IsRound
		{
			if (msg->what == B_SET_PROPERTY)
			{
				bool isRound;
				if (msg->FindBool("data", &isRound) == B_OK)
				{
					SetStyle(isRound ? COLORWELL_ROUND_WELL :
							COLORWELL_SQUARE_WELL);
					status = B_OK;
				}
			}
			else
			if (msg->what == B_GET_PROPERTY)
			{
				reply.AddBool("result", Style() == COLORWELL_ROUND_WELL);
				status = B_OK;
			}
			break;
		}
		case 1: // Red
		{
			if (msg->what == B_SET_PROPERTY)
			{
				int32 newValue;
				if (msg->FindInt32("data", &newValue) == B_OK)
				{
					rgb_color color = ValueAsColor();
					color.red = newValue;
					SetValue(color);
					status = B_OK;
				}
			}
			else
			if (msg->what == B_GET_PROPERTY)
			{
				rgb_color color = ValueAsColor();
				reply.AddInt32("result", color.red);
				status = B_OK;
			}
			break;
		}
		case 2: // Green
		{
			if (msg->what == B_SET_PROPERTY)
			{
				int32 newValue;
				if (msg->FindInt32("data", &newValue) == B_OK)
				{
					rgb_color color = ValueAsColor();
					color.green = newValue;
					SetValue(color);
					status = B_OK;
				}
			}
			else
			if (msg->what == B_GET_PROPERTY)
			{
				rgb_color color = ValueAsColor();
				reply.AddInt32("result", color.green);
				status = B_OK;
			}
			break;
		}
		case 3: // Blue
		{
			if (msg->what == B_SET_PROPERTY)
			{
				int32 newValue;
				if (msg->FindInt32("data", &newValue) == B_OK)
				{
					rgb_color color = ValueAsColor();
					color.blue = newValue;
					SetValue(color);
					status = B_OK;
				}
			}
			else
			if (msg->what == B_GET_PROPERTY)
			{
				rgb_color color = ValueAsColor();
				reply.AddInt32("result", color.blue);
				status = B_OK;
			}
			break;
		}
		default:
			return BControl::MessageReceived(msg);
	}

	if (status != B_OK)
	{
		reply.what = B_MESSAGE_NOT_UNDERSTOOD;
		reply.AddString("message", strerror(status));
	}
	reply.AddInt32("error", status);

	msg->SendReply(&reply);
}


void
ColorWell::Draw(BRect update)
{
	if (fStyle == COLORWELL_SQUARE_WELL)
		DrawSquare();
	else
		DrawRound();
}


rgb_color
ColorWell::ValueAsColor(void) const
{
	return fColor;
}


void
ColorWell::DrawRound(void)
{
	BRect r(Bounds().InsetByCopy(1,1));
	
	if (IsEnabled())
	{
		SetHighColor(fColor);
		FillEllipse(Bounds());
		
		SetHighColor(0,0,0);
		StrokeEllipse(Bounds());
		
	}
	else
	{
		SetHighColor(fDisabledColor);
		FillEllipse(Bounds());
		
		SetHighColor(160,160,160);
		StrokeEllipse(Bounds());
	}
}


void
ColorWell::DrawSquare(void)
{
	BRect r(Bounds());
	
	if (IsEnabled())
	{
		SetHighColor(fColor);
		FillRect(Bounds());
		
		SetHighColor(0,0,0);
		StrokeRect(Bounds());
		
	}
	else
	{
		SetHighColor(fDisabledColor);
		FillRect(Bounds());
		
		SetHighColor(160,160,160);
		StrokeRect(Bounds());
	}
}

