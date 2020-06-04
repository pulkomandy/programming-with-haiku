#include "App.h"
#include "MainWindow.h"


App::App(void)
	:	BApplication("application/x-vnd.dw-TestApp")
{
	MainWindow *mainwin = new MainWindow();
	mainwin->Show();
}

/*
int
main(void)
{

	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
*/

#include <Messenger.h>
#include <Message.h>
#include <String.h>

#include <stdio.h>
/*
int
main(void)
{
	// hey StyledEdit get Title of Window 1
	status_t status;
	BMessenger messenger("application/x-vnd.Haiku-StyledEdit", -1, &status);
	if (status == B_OK)
	{
		BMessage msg(B_GET_PROPERTY), reply;
		msg.AddSpecifier("Title");
		msg.AddSpecifier("Window", 1L);
		if (messenger.SendMessage(&msg, &reply) == B_OK)
		{
			BString title;
			if (reply.FindString("result", &title) == B_OK)
				printf("Title of StyledEdit Window 1: %s\n", title.String());
			else
				printf("Couldn't get title of StyledEdit Window 1\n");
		}
	}
	else
		printf("StyledEdit does not appear to be running.\n");
	
	return 0;
}
*/

#include <Messenger.h>
#include <Message.h>
#include <PropertyInfo.h>
#include <String.h>

#include <stdio.h>

int
main(void)
{
	// hey StyledEdit get Suites of Window 1
	status_t status;
	BMessenger messenger("application/x-vnd.Haiku-StyledEdit", -1, &status);
	if (status == B_OK)
	{
		BMessage msg(B_GET_PROPERTY), reply;
		msg.AddSpecifier("Suites");
		msg.AddSpecifier("Window", 1L);
		if (messenger.SendMessage(&msg, &reply) == B_OK)
		{
			// Now that we have the suites, let's parse them and print them
			// in a format that is a little easier to understand than what
			// BPropertyInfo::PrintToStream() spits out
			int32 i = 0;
			BString suiteName;
			BPropertyInfo propInfo;
			while (reply.FindString("suites", i, &suiteName) == B_OK)
			{
				printf("Suite %s:\n", suiteName.String());
				if (reply.FindFlat("messages", i, &propInfo) != B_OK)
				{
					i++;
					continue;
				}
				
				int32 propCount = propInfo.CountProperties();
				const property_info *info = propInfo.Properties();
				
				for (int32 j = 0; j < propCount; j++)
				{
					BString commands, specifiers;
					
					int32 cmdIndex = 0;
					if (info[j].commands[0] == 0)
						commands = "Get,Set,Count,Create,Delete";
					else
						while (info[j].commands[cmdIndex])
						{
							BString cmdLabel;
							switch (info[j].commands[cmdIndex])
							{
								case B_COUNT_PROPERTIES:
								{
									cmdLabel = "Count";
									break;
								}
								case B_CREATE_PROPERTY:
								{
									cmdLabel = "Create";
									break;
								}
								case B_DELETE_PROPERTY:
								{
									cmdLabel = "Delete";
									break;
								}
								case B_EXECUTE_PROPERTY:
								{
									cmdLabel = "Execute";
									break;
								}
								case B_GET_PROPERTY:
								{
									cmdLabel = "Get";
									break;
								}
								case B_SET_PROPERTY:
								{
									cmdLabel = "Set";
									break;
								}
								default:
									break;
							}
							
							if (cmdLabel.CountChars())
							{
								if (cmdIndex > 0 && commands.CountChars() > 0)
									commands << ",";
								commands << cmdLabel;
							}
							
							cmdIndex++;
						} // end while (commands)
						
					if (commands.CountChars() == 0)
						commands = "None";
					
					int32 specIndex = 0;
					if (info[j].specifiers[0] == 0)
						specifiers = "All";
					else
						while (info[j].specifiers[specIndex])
						{
							BString label;
							switch (info[j].specifiers[specIndex])
							{
								case B_DIRECT_SPECIFIER:
								{
									label = "Direct";
									break;
								}
								case B_NAME_SPECIFIER:
								{
									label = "Name";
									break;
								}
								case B_ID_SPECIFIER:
								{
									label = "ID";
									break;
								}
								case B_INDEX_SPECIFIER:
								{
									label = "Index";
									break;
								}
								case B_REVERSE_INDEX_SPECIFIER:
								{
									label = "ReverseIndex";
									break;
								}
								case B_RANGE_SPECIFIER:
								{
									label = "Range";
									break;
								}
								case B_REVERSE_RANGE_SPECIFIER:
								{
									label = "ReverseRange";
									break;
								}
								default:
									break;
							}
						
							if (label.CountChars())
							{
								if (specIndex > 0 && specifiers.CountChars() > 0)
									specifiers << ",";
								specifiers << label;
							}
							
							specIndex++;
						} // end while(specifiers)
					
					if (specifiers.CountChars() == 0)
						specifiers = "None";
					
					printf("%s: %s (%s)\n", info[j].name, commands.String(),
							specifiers.String());
					
					if (info[j].usage && strlen(info[j].usage) > 0)
						printf("\t%s\n", info[j].usage);
				} // end for each property
				
				printf("\n");
				
				i++;
			} // end for each suite
		}
		
	} // end if status == B_OK
	else
		printf("StyledEdit does not appear to be running.\n");
	
	return 0;
}
