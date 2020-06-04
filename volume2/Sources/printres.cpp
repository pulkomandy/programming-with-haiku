#include <Resources.h>
#include <String.h>
#include <stdio.h>

BString
Int32Code2String(const int32 &code)
{
	// Used to translate 32-bit integers into the 4-character constants
	// often used for formats and BMessage what fields.
	BString out("'");
	out << (char)((code & 0xFF000000) >>  24)
		<< (char)((code & 0x00FF0000) >>  16)
		<< (char)((code & 0x0000FF00) >>  8)
		<< (char)((code & 0x000000FF))
		<< "'";
	return out;
}


void
PrintResources(const char *path)
{
	// This particular version of the BResources constructor is not found in
	// BeOS or Zeta, but it can easily be replaced by another version which takes
	// a BFile object instead.
	BResources res(path);
	if (res.InitCheck() != B_OK)
		return;
	
	// Reading resources from a file follows yet another usage pattern occasionally
	// found in the API: giving a function an index until the read function fails.
	// In this case, we create an index variable, attempt to read the resource at
	// that index, and if we succeed, do what we want with the information obtained,
	// increment the index, and repeat until the call fails. This usage pattern is
	// most commonly done with reading entry_refs from a BMessage.
	int32 resIndex = 0;
	type_code resType;
	int32 resID;
	const char *resName;
	size_t resSize;
	
	printf("Resources in file %s:\n", path);
	bool resourceFound = false;
	while (res.GetResourceInfo(resIndex, &resType, &resID, &resName, &resSize))
	{
		resourceFound = true;
		BString line;
		line << "\t" << Int32Code2String(resType) << "\t" << resID;
		if (resName)
			line << "\t" << resName;
		
		line << "\t" << resSize << " bytes";
		printf("%s\n", line.String());
		resIndex++;
	}
	if (!resourceFound)
		printf("\tNone\n");
}


int
main(int argc, char **argv)
{
	if (argc == 2)
		PrintResources(argv[1]);
	else
		printf("Usage: %s <filename>\n", argv[0]);
	
	return 0;
}
