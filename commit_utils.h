#ifndef COMMIT_UTILS_H
#define COMMIT_UTILS_H

#include "commitnode.h"
#include <string>
using namespace std;

// Functions for commit data I/O.
CommitData readCommitFromFile(string id);
void saveCommitToFile(CommitData data);

// Functions for HEAD management.
string getCurrentHead();
void changeHead(string newHead);

// Function to get the current time.
string getCurrentTime();

// Functions for object file interaction.
string readFileFromObjects(const string& filename, const string& fileHash);
void writeFileToWorkingDirectory(const string& filename, const string& content);

#endif

