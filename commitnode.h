#ifndef COMMITNODE_H
#define COMMITNODE_H

#include <string>
#include <vector>
using namespace std;

// struct: FileBlob
// Description: Represents a specific version of a file (a “blob”)
// Each FileBlob stores the hash of the file’s content and the file’s name
struct FileBlob {
	string fileHash;   // the hash of the file content
	string fileName;   // the name of the file
};

// struct :commitData
// Description :Represents a commit in the MinGit system
// Each commitData stores metadata about the commit (hash,message,time)
// and a list of FileBobs representing the files included in the commit
struct CommitData {
	string commitHash;
	string message;
	string time;
	string parentHash;     	// previous commit in the branch
	string secondParentHash;   // for merge commits
	vector<FileBlob> fileList;
};

#endif

