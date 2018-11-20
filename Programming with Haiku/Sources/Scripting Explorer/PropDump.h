/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef _propdump_h
#define _propdump_h

#include <PropertyInfo.h>
#include <String.h>

enum
{
	MSG_IS_SPEC = 0x00000001,
	MSG_IS_CMD = 0x00000002
};

BString DumpMessage(const BMessage* msg, uint32 dumpFlags = 0,
					int32 pprint = 0);
					
BString DumpMessageField(const BMessage* msg, const char* name,
						type_code type, int32 count, uint32 dumpFlags,
						int32 pprint);

BString DumpPropInfo(const BPropertyInfo* info, int32 pprint);
BString GetCommandName(type_code command);
BString GetSpecName(type_code spec);

#endif /* _propdump_h */
