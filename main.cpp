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