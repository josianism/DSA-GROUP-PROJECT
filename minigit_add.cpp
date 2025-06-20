#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <ctime>
using namespace std;
//Blob:file content
class Blob {
public:
    string filename;
    string fileContent;
    string hash;
};
string computeHash(const string& content) {
    hash<string> hash;
    return to_string(hash(content));
}
struct CommitData {
    string hash;
    string message;
    string time;
    string parentHash;         // previous commit in the branch
    string secondParentHash;   // for merge commits
    vector<Blob> fileList;
};
// Check if the file can be opened and read its contents
string readFile(const string& filename, string& hash) {
    ifstream in(filename);
    if (!in) {
        throw runtime_error("Cannot open file: " + filename);
    }
    stringstream buffer;
    buffer << in.rdbuf();
    string content=buffer.str();
    // Compute the hash
    hash=computeHash(content);

    return content;
}
// Existance of the file
//To avoid storing duplicate
bool fileExists(const string& filepath) {
    return filesystem::exists(filepath);
}
// Stage files for the next commit
void updateIndex(const string& filename, const string& hash) {
    ofstream indexFile(".minigit/index", ios::app);
    indexFile << filename << " : " << hash << endl;
    indexFile.close();
}
//Add file
void addFile(const string& filename) {
    string hash;
    string content=readFile(filename,hash);
    Blob blob;
    blob.filename=filename;
    blob.hash=computeHash(content);
    string objectPath = ".minigit/objects/" + blob.hash;
    // Write blob only if it doesn't already exist
    if (!fileExists(objectPath)) {
        writeFile(objectPath, content);
    }
    // Update the index
    updateIndex(filename, blob.hash);
    cout << "Added " << filename << " with hash " << blob.hash << endl;
}
void writeFile(const string& filepath, const string& content) {
    ofstream filestream(filepath);
    if(!filestream){
        throw runtime_error("Cannot open file");
    }
    filestream << content;
    filestream.close();
}
CommitData readCommitFromFile(string id) {
    CommitData c;
    ifstream file(".minigit/commits/" + id);
    if (!file.is_open()) {
        cout << "Cannot find commit with ID: " << id << "\n";
        return c;
    }
    getline(file, c.message);
    getline(file, c.time);
    getline(file, c.parentHash);
    getline(file, c.secondParentHash);
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        Blob f;
        ss >> f.hash >> f.filename;
        c.fileList.push_back(f);
    }
    c.hash = id;
    return c;
}
// saves commit data to a file.
void saveCommitToFile(CommitData data) {
    ofstream file(".minigit/commits/" + data.hash);
    file << data.message << "\n";
    file << data.time << "\n";
    file << data.parentHash << "\n";
    file << data.secondParentHash << "\n";

    for (auto f : data.fileList) {
        file << f.hash << " " << f.filename << "\n";
    }
}
CommitData findBaseCommit(CommitData one, CommitData two) {
    unordered_map <string, bool> seen;

    while (!one.hash.empty()) {
        seen[one.hash] = true;
        one = readCommitFromFile(one.parentHash);
    }

    while (!two.hash.empty()) {
        if (seen[two.hash]) {
            return readCommitFromFile(two.hash);
        }
        two = readCommitFromFile(two.parentHash);
    }

    return CommitData{};
}
// Retrieves the current HEAD commit ID.
string getCurrentHead() {
    ifstream file(".minigit/HEAD");
    string line;
    getline(file, line);
    file.close();

    return line;
}
// changes the head to a new commit ID.
void changeHead(string newHead) {
    ofstream file(".minigit/HEAD");
    file << newHead;
}
// Gets the current date and time as a formatted string.
string getCurrentTime() {
    time_t now = time(0);
    char buff[80];
    strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buff);
}
void commit(const string& message) {
    ifstream index(".minigit/index");
    if (!index) {
        cout << "No staged files to commit.\n";
        return;
    }
    // Prepare CommitData
    CommitData newCommit;
    newCommit.message = message;
    newCommit.time = getCurrentTime();
    // Set parent hash
    string parentHash = getCurrentHead();
    newCommit.parentHash = parentHash;
    newCommit.secondParentHash = ""; // Not a merge
    string line;
    while (getline(index, line)) {
        size_t delimiter = line.find(" : ");
        if (delimiter != string::npos) {
            string filename = line.substr(0, delimiter);
            string hash = line.substr(delimiter + 3);

            Blob file;
            file.filename = filename;
            file.hash = hash;
            newCommit.fileList.push_back(file);
        }
    }
    index.close();
    // Create unique commit hash (timestamp-based)
    newCommit.hash = computeHash(newCommit.message + newCommit.time);
    filesystem::create_directories(".minigit/commits");
    saveCommitToFile(newCommit);
    changeHead(newCommit.hash);
    // Clear the index
    ofstream clear(".minigit/index", ios::trunc);
    clear.close();
    cout <<message <<" successfully committed." << endl;
}
int main() {
    string filename;
    string message;
    addFile(filename);
    commit(message);
}