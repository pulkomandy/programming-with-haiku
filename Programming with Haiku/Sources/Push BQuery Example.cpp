#include <Entry.h>
#include <Query.h>
#include <stdio.h>
#include <String.h>
#include <Volume.h>
#include <VolumeRoster.h>

int
main(void)
{
	// Get a BVolume object which represents the boot volume. A BQuery
	// object will require one.
	BVolumeRoster volRoster;
	BVolume bootVolume;
	volRoster.GetBootVolume(&bootVolume);
	
	// A quick summary for using BQuery:
	// 1) Make the BQuery object.
	// 2) Set the target volume with SetVolume().
	// 3) Specify the search terms with SetPredicate().
	// 4) Call Fetch() to start the search
	// 5) Iterate over the results list using GetNextEntry(),
	//	GetNextRef(), or GetNextDirents().
	
	BString predicate("MAIL:subject == *");
	BQuery query;
	query.SetVolume(&bootVolume);
	query.PushAttr("MAIL:subject");
	query.PushString("*");
	query.PushOp(B_EQ);
	if (query.Fetch() == B_OK)
	{
		printf("Results of query \"%s\":\n", predicate.String());
		entry_ref ref;
		while (query.GetNextRef(&ref) == B_OK)
			printf("\t%s\n",ref.name);
	}
}

