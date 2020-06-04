// This is intended to be a cross-platform way of reading Paladin
// projects, so we won't be using the Haiku API. This is strictly
// C++ methods.

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <strings.h>
#include <vector>

// Because we're doing so much in the std namespace, this will save
// us a *lot* of extra typing.
using namespace std;

// True, these could (and probably should) be classes, we will just
// use structures for the sake of space and simplicity.
typedef struct
{
	string path;
	string dependencies;

} ProjectFile;


typedef struct
{
	string name;
	bool expanded;
	vector<ProjectFile> files;

} ProjectGroup;


// The actual Project class used by Paladin is much more
// complicated because it also has some properties for
// maintaining state while the program is running, but
// this structure has all of the data that is used for
// building and maintaining a project.
typedef struct
{
	map<string,string>		properties;
	
	vector<string>			localIncludes;
	vector<string>			systemIncludes;
	vector<ProjectGroup>	groups;
	vector<string>			libraries;

} PaladinProject;


int
ReadPaladinProject(const char *path, PaladinProject &proj)
{
	// Create an Input File Stream for reading the file
	ifstream file;
	
	file.open(path, ifstream::in);
	if (!file.is_open())
	{
		// endl is a cross-platform stand-in for an end-of-line
		// character. The EOL character is different on Windows,
		// Haiku/Linux/UNIX, and OS X and this saves us from
		// having to figure it out without sacrificing portability.
		cout << "Couldn't open the file " << path << endl;
		return -1;
	}
	
	// Empty the project's data to make sure we're not building
	// upon existing baggage.
	proj.properties.clear();
	proj.localIncludes.clear();
	proj.systemIncludes.clear();
	proj.groups.clear();
	
	while (!file.eof())
	{
		string strData;
		
		// While the fstream class has a getline() method, it only
		// works on regular strings. There is a global getline()
		// function in <string> which reads data from a stream into
		// a C++ string. This is the version that we will use.
		getline(file, strData);
		
		// An empty line shouldn't exist in a Paladin project, but
		// let's handle the case just to prevent headaches.
		if (strData.empty())
			continue;
		
		size_t pos = strData.find('=');
		
		// npos is the maximum size for a C++ string. find() will
		// return npos if the string searched for is not found.
		if (pos == string::npos)
			continue;
		
		string key = strData.substr(0, pos);
		string value = strData.substr(pos + 1, string::npos);
		
		if (key.compare("GROUP") == 0)
		{
			// Using vectors saves us from having to worry about memory
			// management and pointers. Instead, all that we have to do
			// is create a new group on the stack which will be used
			// to initialize the new element in the groups vector.
			ProjectGroup newGroup;
			newGroup.name = value;
			proj.groups.push_back(newGroup);
			
		}
		else if (key.compare("EXPANDGROUP") == 0)
		{
			if (!proj.groups.empty())
				proj.groups.back().expanded = strcasecmp(value.c_str(), "yes");
		}
		else if (key.compare("SOURCEFILE") == 0)
		{
			// Quite a few dots are used to create the new file, but that's OK.
			// It sure beats messing around with pointers.
			ProjectFile newFile;
			newFile.path = value;
			proj.groups.back().files.push_back(newFile);
		}
		else if (key.compare("DEPENDENCY") == 0)
			proj.groups.back().files.back().dependencies = value;
		else if (key.compare("LIBRARY") == 0)
			proj.libraries.push_back(value);
		else
			proj.properties[key] = value;
	}
	
	return 0;
}


int
main(int argc, char **argv)
{
	PaladinProject project;
	
	if (argc == 2)
		ReadPaladinProject(argv[1], project);
	else
		cout << "Usage: " << argv[0] << " <path>\n";
	
	map<string,string>::iterator i;
	for (i = project.properties.begin(); i != project.properties.end(); i++)
		cout << i->first << ": " << i->second << endl;
	
	return 0;
}

