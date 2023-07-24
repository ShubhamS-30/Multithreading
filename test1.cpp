#include <iostream>
#include <thread>
#include <vector>
#include<bits/stdc++.h>

void threadFunction(int threadId) {
    std::cout << "Thread " << threadId << " is running." << std::endl;
}
// void task(string target_folder,vector<string> &source_files,string destination_folder)
// {
//     string target_file,dest_file;
//     for(auto x:source_files)
//     {
//         target_file = target_folder + x;
//         dest_file = destination_folder + x;
//         CopyFile(target_file.c_str(),dest_file.c_str(),true);
//         c++;
//     }
// }
int main() {
    int n = 5; // Number of threads to create
    std::vector<std::thread> threads;

    // Create threads
    for (int i = 0; i < n; ++i) {
        threads.push_back(std::thread(threadFunction, i));
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "All threads have finished executing." << std::endl;

    return 0;
}
