void log(){
    ifstream current(".minigit/HEAD");
    int logNum=0;

    string curHash;
    getline(current,curHash);
    current.close();
    if(curHash.empty()){
        cout<<"There are no commits."<<endl;
        return;
    }
    if (curHash.empty() || curHash.find("ref:") != string::npos) {
        cout << "There are no commits." << endl;
        return;
    }
    while(!curHash.empty()){
        CommitData newcommit=readCommitFromFile(curHash);
        if(newcommit.message.empty()) break;
        cout<<"Commit #"<<logNum++<<" : "<<newcommit.message<<endl;
        //gives a number to each commit and displays it with the message
        cout<<"Commit Time: "<<newcommit.time<<endl;
        cout<<"Commit ID: "<<newcommit.hash<<endl;
        cout<<"Commit Parent: "<<(newcommit.parentHash.empty()? "None": newcommit.parentHash)<<endl;

        if (!newcommit.secondParentHash.empty()) {
            cout << "  Merge Parent: " << newcommit.secondParentHash << endl;
        }
        for(const auto& file: newcommit.fileList){
            cout<<file.filename<<" : "<<file.hash<<endl;
        }
        curHash=newcommit.parentHash;
    }
}

void createBranch(const string& branchName) {
    string currentHash = getCurrentHead();
    ofstream out(".minigit/refs/heads/" + branchName);
    if (!out) {
        cerr << "Failed to create branch file.\n";
        return;
    }
    out << currentHash << endl;
    out.close();
    cout << "Branch '" << branchName << "' created at commit " << currentHash << endl;
}
void checkoutBranch(const string& branchName) {
    string refPath = ".minigit/refs/heads/" + branchName;
    if (!fileExists(refPath)) {
        cerr << "Branch does not exist.\n";
        return;
    }

    ofstream head(".minigit/HEAD");
    head << "ref: refs/heads/" << branchName << endl;
    cout << "Switched to branch '" << branchName << "'\n";
}
