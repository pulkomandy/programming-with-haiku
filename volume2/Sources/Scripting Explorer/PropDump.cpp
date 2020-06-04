/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#include <stdio.h>
#include <Message.h>
#include <PropertyInfo.h>
#include <ByteOrder.h>
#include "PropDump.h"

BString
DumpMessage(const BMessage* msg, uint32 dumpFlags, int32 pprint)
{
	BString str = "message what = ";
	BString ptab1, ptab2;

	char whatstr[11];
	sprintf(whatstr, "%#010lx", msg->what);
	
	for (int32 n=0; n < pprint; n++)
		ptab1 += "\t";
	
	ptab2 = ptab1;
	ptab2 += "\t";
	
	if (dumpFlags & MSG_IS_SPEC)
	{
		BString cmdStr = GetSpecName(msg->what);
		str += cmdStr;
	}
	else if (dumpFlags & MSG_IS_CMD)
	{
		BString cmdStr = GetCommandName(msg->what);
		str += cmdStr;
	}
	else
	{
		char typestr[5];
		int32 swap = B_BENDIAN_TO_HOST_INT32(msg->what);
		memcpy(typestr, &swap, 4);
		typestr[4] = '\0';
		str << whatstr << " ('" << typestr << "')";
	}

	str += " {\n";
	
	int32 i=0;
	int32 count, nameCount = msg->CountNames(B_ANY_TYPE);
	char *fieldName;
	type_code type;
	while (msg->GetInfo(B_ANY_TYPE, i++, &fieldName, &type, &count) == B_OK)
	{
		str << ptab2 << fieldName << " = ";
		if (count == 1)
		{
			BString myStr = DumpMessageField(msg, fieldName, type, count-1, 0, pprint+1);
			str << myStr;
		}
		else
		{
			str << "{\n";
			for (int32 j=0; j<count; j++)
			{
				BString myStr = DumpMessageField(msg, fieldName, type, j, 0, pprint+2);
				str << ptab2 << "[" << j << "]:\t" << myStr;
				if (j != count - 1)
					str << ",";
				str << "\n";
			}
			str << ptab2 << "}";
		}
		if (i != nameCount)
			str << ", ";
		str << "\n";
	}
	str << ptab1 << "}";
	return str;
}


BString
DumpMessageField(const BMessage* msg, const char* name, type_code type,
				int32 count, uint32 dumpFlags, int32 pprint)
{
	BString str;
	switch (type)
	{
		case B_STRING_TYPE:
		{
			str << "\"" << msg->FindString(name, count) << "\"";
			break;
		}
		case B_INT32_TYPE:
		{
			int32 val = msg->FindInt32(name, count);
			if (! strcmp(name, "error"))
				str = strerror(val);
			else
				str << val << "L";
			break;
		}
		case B_MESSAGE_TYPE:
		{
			BMessage inMsg;
			msg->FindMessage(name, count, &inMsg);
			if (! strcmp(name, "specifiers"))
				dumpFlags = dumpFlags | MSG_IS_SPEC;
			BString msgStr = DumpMessage(&inMsg, dumpFlags, pprint);
			str = msgStr;
			break;
		}
		case B_PROPERTY_INFO_TYPE:
		{
			const void* data;
			ssize_t size;
			status_t err = msg->FindData(name, B_PROPERTY_INFO_TYPE, count, &data, &size);
			if (err == B_OK)
			{
				BPropertyInfo info;
				if (info.Unflatten(B_PROPERTY_INFO_TYPE, data, size) == B_OK)
				{
					BString msgStr = DumpPropInfo(&info, pprint);
					str = msgStr;
				}
				else
					printf("Couldn't decode prop info!\n");
			}
			else
				printf("Lost the prop info data!\n");
			break;
		}	
		default:
		{	
			int32 swap = B_BENDIAN_TO_HOST_INT32(type);
			char typestr[5];
			typestr[4] = '\0';
			memcpy(typestr, &swap, 4);
			str << "data ('" << typestr << "')";
			break;
		}
	}
	return str;
}


BString
DumpPropInfo(const BPropertyInfo* info, int32 pprint)
{
	BString str = "prop_info = {\n";
	BString ptab1, ptab2, ptab4, ptab5;

	for (int32 n=0; n < pprint; n++)
		ptab1 += "\t";
	
	ptab2 = ptab1;
	ptab2 += "\t";
	ptab4 << ptab2 << "\t\t";
	ptab5 << ptab4 << "\t";
	
	int32 count = info->CountProperties();
	if (count > 0)
	{
		str << ptab2 << "properties = {\n";
		const property_info* props = info->Properties();
		for (int32 i=0; i<count; i++)
		{
			int32 j = 0;
			uint32 val;
			const property_info* pp = props + i;
			str << ptab2 << "[" << i << "]:\tproperty = {\n";
			str << ptab4 << "name = \"" << pp->name << "\",\n";
			str << ptab4 << "commands = {\n";
			while ((val = pp->commands[j++]))
			{
				BString cmdStr = GetCommandName(val);
				str << ptab5 << cmdStr;
				if (pp->commands[j])
					str << ",";
				str << "\n"; 
			}
			str << ptab4 << "},\n"; 
			str << ptab4 << "specifiers = {\n";
			j = 0;
			while ((val = pp->specifiers[j++]))
			{
				BString specStr = GetSpecName(val);
				str << ptab5 << specStr;
				if (pp->specifiers[j])
					str << ",";
				str << "\n";
			}
			str << ptab4 << "},\n";
			str << ptab4 << "usage = \"" << pp->usage << "\"\n";
			
			char datastr[11];
			sprintf(datastr, "%#010lx", pp->extra_data);
			str << ptab4 << "extra data = " << datastr << "\n";
			str << ptab2 << "\t}";
			if (i != count - 1) {
				str << ",";
			}
			str << "\n";
		}
		str << ptab2 << "},\n";
	} else
	str << ptab2 << "properties = 0,\n";
	
	count = info->CountValues();
	if (count > 0)
	{
		str << ptab2 << "values = {\n";
		const value_info* values = info->Values();
		for (int32 i=0; i<count; i++)
		{
			const value_info* pv = values + i;
			char datastr[11];
			str << ptab2 << "[" << i << "]:\value {\n";
			str << ptab4 << "name = \"" << pv->name << "\",\n";
			sprintf(datastr, "%#010lx", pv->value);
			str << ptab4 << "value = " << datastr << "\n";
			str << ptab4 << "usage = \"" << pv->usage << "\"\n";		
			sprintf(datastr, "%#010lx", pv->extra_data);
			str << ptab4 << "extra data = " << datastr << "\n";
			str << ptab2 << "\t}";
			if (i != count - 1)
				str << ",";
			str << "\n";
		}
		str << ptab2 << "}\n";
	}
	else
		str << ptab2 << "values = 0\n";
	str << ptab1 << "}";
	return str;	
}


BString
GetCommandName(type_code command)
{
	// these come from AppDefs.h
	BString str;
	switch (command)
	{
		case B_SET_PROPERTY:
			str = "B_SET_PROPERTY";
			break;
		case B_GET_PROPERTY:
			str = "B_GET_PROPERTY";
			break;
		case B_CREATE_PROPERTY:
			str = "B_CREATE_PROPERTY";
			break;
		case B_DELETE_PROPERTY:
			str = "B_DELETE_PROPERTY";
			break;
		case B_COUNT_PROPERTIES:
			str = "B_COUNT_PROPERTIES";
			break;
		case B_EXECUTE_PROPERTY:
			str = "B_EXECUTE_PROPERTY";
			break;
		case B_GET_SUPPORTED_SUITES:
			str = "B_GET_SUPPORTED_SUITES";
			break;
		case B_MESSAGE_NOT_UNDERSTOOD:
			str = "B_MESSAGE_NOT_UNDERSTOOD";
			break;
		case B_NO_REPLY:
			str = "B_NO_REPLY";
			break;
		case B_REPLY:
			str = "B_REPLY";
			break;
		default:
		{
			char whatstr[11];
			sprintf(whatstr, "%#010lx", command);
			char typestr[5];
			int32 swap = B_BENDIAN_TO_HOST_INT32(command);
			memcpy(typestr, &swap, 4);
			typestr[4] = '\0';
			str << whatstr << " ('" << typestr << "')";
			break;
		}
	}
	return str;
}


BString
GetSpecName(type_code spec)
{
	BString str;
	switch (spec)
	{
		case B_DIRECT_SPECIFIER:
			str = "B_DIRECT_SPECIFIER";
			break;
		case B_INDEX_SPECIFIER:
			str = "B_INDEX_SPECIFIER";
			break;
		case B_NAME_SPECIFIER:
			str = "B_NAME_SPECIFIER";
			break;
		case B_REVERSE_INDEX_SPECIFIER:
			str = "B_REVERSE_INDEX_SPECIFIER";
			break;
		case B_RANGE_SPECIFIER:
			str = "B_RANGE_SPECIFIER";
			break;
		case B_REVERSE_RANGE_SPECIFIER:
			str = "B_REVERSE_RANGE_SPECIFIER";
			break;
		case B_ID_SPECIFIER:
			str = "B_ID_SPECIFIER";
			break;
		default:
		{
			char whatstr[11];
			sprintf(whatstr, "%#010lx", spec);
			char typestr[5];
			int32 swap = B_BENDIAN_TO_HOST_INT32(spec);
			memcpy(typestr, &swap, 4);
			typestr[4] = '\0';
			str << whatstr << " ('" << typestr << "')";
			break;
		}
	}
	return str;
}
