# MiniGit - Simple Git System in C++

This is a basic version control system built in C++.  
We made a smaller version of Git to understand how it works behind the scenes.  
This project helped us practice file handling, hash generation, branching, merging, and working as a team.

#Project Structure

When we run `init`, MiniGit creates a hidden folder `.minigit` with:

- `HEAD` – tracks the current branch  
- `refs/heads/` – contains branch names and their latest commit  
- `objects/` – stores file versions based on their hash  
- `commits/` – stores all commits  
- `index` – tracks staged files before committing  

# How to Compile and Run

1. Compile the code:  
   ```bash
   g++ -std=c++17 -o minigit main.cpp
2.Run the program
./minigit
3.Use commands like:

init
add <filename>
commit <your message>
log
branch <name>
checkout <branch> or <commit-hash>
merge <branch>
exit

Merge Conflicts

If two branches change the same file, MiniGit adds conflict markers like this:

<<<<<<< Current Branch
Your version of the file
=======
Other branch’s version
>>>>>>> other-branch

To resolve:

-Open the file

-Choose which version to keep (or combine both)

-Remove the markers

-Save and commit the file manually

Team Members and Roles

1.Nebay Haile – MiniGit setup, init, hash logic
2.Eyosias Tsegaye – Checkout, testing, documentation, repository
3.Yelibe Tsedeke – Staging and commit
4.Tewobsta Migbar – Log and branching
5.Tsedalemariam Getu – Merge and utility functions

Here is what We Learned from this project

-How Git internally tracks files and changes

-How to use C++ maps, vectors, and file streams

-How to work together on GitHub

-How to simulate branching and merging

-How to solve merge conflicts

Overall, this project helped us learn how version control works.
It doesn’t have all features of Git but gave us a great understanding of the core ideas.

Thank you for reviewing our MiniGit project!


# DSA-GROUP-PROJECT
