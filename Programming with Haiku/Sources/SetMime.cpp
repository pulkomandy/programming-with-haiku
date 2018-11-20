#include <Message.h>
#include <Mime.h>

int
main(void)
{
	BMimeType mime("application/x-person");
	BMessage attrMsg;
	
	// We get the existing information for Person files because otherwise
	// we will replace it with the two measly attributes that we have below.
	// We want to add to the existing attributes for Person files, not
	// replace them.
	mime.GetAttrInfo(&attrMsg);

	// Each of these fields needs to be added to the message for the
	// custom attribute to be useful.
	attrMsg.AddString("attr:public_name", "First Name"); 
	attrMsg.AddString("attr:name", "META:firstname"); 
	attrMsg.AddInt32("attr:type", B_STRING_TYPE); 
	attrMsg.AddBool("attr:viewable", true); 
	attrMsg.AddBool("attr:editable", true); 

	// These three fields are not documented in the Be Book, but are used by
	// Tracker to determine how the information is displayed in a Tracker
	// window.
	attrMsg.AddInt32("attr:width", 120); 
	attrMsg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
	attrMsg.AddBool("attr:extra", false); 
				 
	attrMsg.AddString("attr:public_name", "Last Name"); 
	attrMsg.AddString("attr:name", "META:lastname"); 
	attrMsg.AddInt32("attr:type", B_STRING_TYPE); 
	attrMsg.AddBool("attr:viewable", true); 
	attrMsg.AddBool("attr:editable", true); 
	attrMsg.AddInt32("attr:width", 120); 
	attrMsg.AddInt32("attr:alignment", B_ALIGN_LEFT); 
	attrMsg.AddBool("attr:extra", false); 

	mime.SetAttrInfo(&attrMsg);
}
