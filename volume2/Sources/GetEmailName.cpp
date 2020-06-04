#include <fs_attr.h>
#include <Node.h>
#include <String.h>

BString
GetEmailName(const char *path)
{
	BString out;
	BNode node(path);
	if (node.InitCheck() != B_OK)
		return out;
	
	// This is to make sure that the attribute exists and what
	// its size is.
	attr_info attrInfo;
	if (node.GetAttrInfo("META:name", &attrInfo) != B_OK)
		return out;
	
	// BString::LockBuffer() and UnlockBuffer() allow us to have
	// direct access to the internal character buffer that the BString
	// uses. LockBuffer() takes one parameter which is the maximum
	// size that the character buffer needs to be.
	char *nameBuffer = out.LockBuffer(attrInfo.size + 1);
	node.ReadAttr("META:name", attrInfo.type, 0, nameBuffer,
			attrInfo.size);
	nameBuffer[attrInfo.size] = '\0';
	out.UnlockBuffer();
	
	return out;
}

int
main(void)
{
}
