#include "merge.h"
#include "commit_utils.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream> // For stringstream
using namespace std;
// Finds the lowest common ancestor commit between two commits
CommitData findBaseCommit(CommitData one, CommitData two) {
    map<string, bool> seen;

    // Traverse parent chain from one branch
    while (!one.commitHash.empty()) {
        seen[one.commitHash] = true;
        one = readCommitFromFile(one.parentHash);
    }

    // Traverse parent chain from the other branch and find first match
    while (!two.commitHash.empty()) {
        if (seen[two.commitHash]) {
            return readCommitFromFile(two.commitHash); // LCA found
        }
        two = readCommitFromFile(two.parentHash);
    }

    // If no common ancestor is found
    return CommitData{};
}

void mergeFromBranch(string otherBranch) {
    // Get the current HEAD commit
    string currentHash = getCurrentHead();
    CommitData current = readCommitFromFile(currentHash);

    // Open the branch file to get the latest commit of the other branch
    ifstream file(".minigit/refs/heads/" + otherBranch);
    string otherHash;
    if (!file.is_open()) {
        cerr << "Error: Could not open branch file: " << otherBranch << endl;
        return;
    }
    getline(file, otherHash); // Read commit hash of other branch
    CommitData other = readCommitFromFile(otherHash);

    // Find the lowest common ancestor (base commit)
    CommitData base = findBaseCommit(current, other);

    // Create maps for file comparison
    map<string, string> baseMap, currentMap, otherMap, result;
    bool conflict = false;

    // Fill file hash maps from base, current, and other branch
    for (auto& f : base.fileList) baseMap[f.fileName] = f.fileHash;
    for (auto& f : current.fileList) currentMap[f.fileName] = f.fileHash;
    for (auto& f : other.fileList) otherMap[f.fileName] = f.fileHash;

    // Compare files between current and other using base
    for (auto [fname, oHash] : otherMap) {
        string cHash = currentMap[fname];
        string bHash = baseMap[fname];

        // Conflict: both changed differently from base
        if (cHash != oHash && bHash != cHash && bHash != oHash) {
            cout << "CONFLICT: File changed in both: " << fname << "\n";
            conflict = true;

            string currentContent = readFileFromObjects(fname, cHash);
            string otherContent = readFileFromObjects(fname, oHash);

            if (currentContent.empty() || otherContent.empty()) {
                cerr << "Error reading file content for merge: " << fname << endl;
                continue;
            }

            // Create and write conflict marker
            string conflictMarker = "<<<<<<< HEAD\n" + currentContent +
                                    "=======\n" + otherContent +
                                    ">>>>>>> " + otherBranch + "\n";
            writeFileToWorkingDirectory(fname, conflictMarker);
        }

        // Only changed in other → safe to take
        else if (cHash == bHash && oHash != bHash) {
            string otherContent = readFileFromObjects(fname, oHash);
            writeFileToWorkingDirectory(fname, otherContent);
            result[fname] = oHash;
        }

        // Same in both or unchanged → keep current
        else {
            result[fname] = cHash.empty() ? oHash : cHash;
        }
    }

    // Add remaining files that exist only in current branch
    for (auto [fname, cHash] : currentMap) {
        if (result.find(fname) == result.end()) {
            result[fname] = cHash;
        }
    }

    // If no conflict, create new merged commit
    if (!conflict) {
        CommitData newCommit;
        newCommit.parentHash = current.commitHash;
        newCommit.secondParentHash = other.commitHash;
        newCommit.message = "Merge from branch " + otherBranch;
        newCommit.time = getCurrentTime();
        newCommit.commitHash = current.commitHash.substr(0, 4) + other.commitHash.substr(0, 4);

        for (auto [fname, fhash] : result) {
            FileBlob f;
            f.fileName = fname;
            f.fileHash = fhash;
            newCommit.fileList.push_back(f);
        }

        saveCommitToFile(newCommit);
        changeHead(newCommit.commitHash);
        cout << "Merge successful. New commit is: " << newCommit.commitHash << "\n";
    } else {
        cout << "There were conflicts. Fix them and commit manually.\n";
    }
}


