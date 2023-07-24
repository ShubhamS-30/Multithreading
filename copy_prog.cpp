#include <bits/stdc++.h>
#include <thread>
#include <dirent.h>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <windows.h>
using namespace std;
void task(int l, int r, string target_folder, vector<string> &source_files, string destination_folder)
{
    string target_file, dest_file;
    for (int i = l; i <= r; i++)
    {
        target_file = target_folder + source_files[i];
        dest_file = destination_folder + source_files[i];
        CopyFile(target_file.c_str(), dest_file.c_str(), true);
    }
}
float Multithreading(int file_num, vector<string> &source_files, string target_folder, string destination_folder)
{
    // to be implemented
    int l, r, batch_size;
    l = 0;
    r = min(file_num,int(source_files.size()));
    

    int n = 5; // Number of threads to create

    batch_size = file_num/n;
   

    // while (l <= r)
    // {
        vector<thread> threads;
        for (int i = 0; i < n; ++i)
        {
            threads.push_back(thread(task, l, l + batch_size - 1, target_folder, source_files, destination_folder));
            l += batch_size;
        }
        for (auto &thread : threads)
        {
            thread.join();
        }
    // }
    // // Create threads
    // for (int i = 0; i < n; ++i)
    // {
    //     threads.push_back(thread(threadFunction, i));
    // }

    // // Join threads
    // for (auto &thread : threads)
    // {
    //     thread.join();
    // }

    cout << "All threads have finished executing." << endl;
    return 0.0;
}
int main()
{
    // copy file from one place to another
    // CopyFile("./IMG/2.jpg","./demo/2.jpg",true);

    // thread t1(fun1, "X");
    // thread t2(fun1, "O");
    // t2.join();
    // t1.join();

    // write to csv
    // vector<int> v = {1,2,3,4,5,6};
    // ofstream output_file("test_c++.csv");
    // for(int i = 0;i<v.size();i++)
    // {
    //     output_file<<v[i]<<endl;
    // }
    auto begin = std::chrono::high_resolution_clock::now();
    vector<string> source_files;
    string target_folder = "./IMG/";
    DIR *dr;
    struct dirent *en;
    string file;
    dr = opendir(target_folder.c_str()); // open all directory
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            file = en->d_name;
            if (file.size() > 3 and file.substr(file.size() - 3) == "jpg")
                source_files.push_back(file);
        }
        closedir(dr); // close all directory
    }
    cout << source_files.size() << endl;
    // for (int i = 0; i <= 100; i++)
    //     cout << source_files[i] << endl;

    // task fun working
    // task(0, 100, target_folder, source_files, "./demo/");
    cout<<"A"<<endl;
    float x = Multithreading(1000,source_files,target_folder,"./demo/");
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << "time = " << elapsed.count() * 1e-9 << endl;
    return 0;
}