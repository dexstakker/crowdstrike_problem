//
//  main.cpp
//  CrowdStrikeProj
//
//  Created by Jason Graham on 3/13/24.
//

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <mutex>
#include <thread>
#include <map>

using namespace std;
namespace fs = std::filesystem;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

mutex gLock;
vector<string> results;
string basedir;
vector<thread> spool;
map<string,recursive_directory_iterator> itermap;


void addFileEntry(const string& val) {
    lock_guard<mutex> lockGuard(gLock);
    results.push_back(val);
}

void dumpAndClearResults() {
    lock_guard<mutex> lockGuard(gLock);
    for (string val: results) {
        cout << "Next Valid Entry = " << val << endl;
    }
    results.clear();
}

void handleSearch( string val ) {
    
    cout << "HANDLE SEARCH for " << val << endl;
    for (const auto& dirEntry : itermap[val]) {
        string dirEntryPath = dirEntry.path();
        const size_t last_slash_idx = dirEntryPath.find_last_of("\\/");
        if (std::string::npos != last_slash_idx)
        {
            dirEntryPath.erase(0, last_slash_idx + 1);
        }
        if (dirEntryPath.find(val) != std::string::npos) {
            addFileEntry(dirEntryPath);
            //std::cout << dirEntryPath << std::endl;
        }
    }
    dumpAndClearResults();
}

int main(int argc, const char * argv[]) {
    

    // Allocate results vector
    cout << "Hello, World!\n";
    string path = "/Users/dex/Documents/c++src/CrowdStrikeProj/CrowdStrikeProj/";
    vector<string> criteria;
    
    
    for (int i = 1; i < argc; i++) {
        string eachVal(argv[i]);
        if (i == 1) {
            basedir = string(argv[i]);
            path += basedir;
        } else {
            criteria.push_back(eachVal);
            itermap.insert ( std::pair<string,recursive_directory_iterator>(eachVal,recursive_directory_iterator(path)) );
        }
    }
    std::thread t;
    for (string val: criteria) {
        //std::thread t(handleSearch, val);
        t = std::thread(handleSearch, val);
        t.join();
        
    }
    
    cout << "Threads to be erased" << endl;
    
    // release results vector
    results.clear();
    return 0;
}
