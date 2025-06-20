
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;

string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) return "";
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

void writeFile(const string& path, const string& content) {
    ofstream file(path);
    file << content;
}

string hashContent(const string& content) {
    hash<string> hasher;
    size_t hashed = hasher(content);
    stringstream ss;
    ss << hex << hashed;
    return ss.str();
}

void init() {
    fs::create_directory(".minigit");
    fs::create_directory(".minigit/objects");
    fs::create_directory(".minigit/refs");
    ofstream head(".minigit/HEAD");
    head << "ref: refs/heads/main\n";
    ofstream index(".minigit/index");
    cout << "MiniGit initialized successfully.\n";
}

class Blob {
public:
    string fileContent;
    string hash;
};

class CommitNode {
public:
    string message;
    CommitNode* parent;
    CommitNode(const string& msg) : message(msg), parent(nullptr) {}
};
