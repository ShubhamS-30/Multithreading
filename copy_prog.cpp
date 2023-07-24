#include <bits/stdc++.h>
#include <thread>
#include <dirent.h>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <windows.h>
using namespace std;
void task(int l, int r, string target_folder, vector<string> source_files, string destination_folder)
{
    string target_file, dest_file;
    for (int i = l; i <= r; i++)
    {
        target_file = target_folder + source_files[i];
        dest_file = destination_folder + source_files[i];
        CopyFile(target_file.c_str(), dest_file.c_str(), true);
    }
}
float Multithreading(int num_threads, int file_num, vector<string> &source_files, string target_folder, string destination_folder)
{
    if (mkdir(destination_folder.c_str()) == 1)
    {
        cout << "FAILED TO CREATE DESTINATION FOLDER" << endl;
        return -1;
    }

    int l, r, batch_size;
    l = 0;
    r = min(file_num, int(source_files.size()));
    batch_size = file_num / num_threads;

    if (batch_size == 0)
    {
        cout << "Number of Images should be greater than the number of threads created" << endl;
        return -1;
    }
    int n = num_threads;

    auto begin = std::chrono::high_resolution_clock::now();

    vector<thread> threads;
    for (int i = 0; i < n; ++i)
    {
        if (i < n - 1)
            threads.push_back(thread(task, l, l + batch_size - 1, target_folder, source_files, destination_folder));
        else
            threads.push_back(thread(task, l, r - 1, target_folder, source_files, destination_folder));
        l += batch_size;
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    // All threads have finished executing.
    return elapsed.count() * 1e-9;
}
int main()
{
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
    cout << "Number of files in source folder = " << source_files.size() << endl;

    string destination_folder;
    vector<vector<float>> results;
    for (int i = 1; i <= 11; i++)
    {
        destination_folder = "./destination_" + to_string(i) + '/';
        float x = Multithreading(i, 10000, source_files, target_folder, destination_folder);
        results.push_back({float(i), x});
    }
    for (auto x : results)
    {
        cout << x[0] << " " << x[1] << endl;
    }
    std::ofstream out("copy_images_cpp.csv");
    out << "Threads" << ',' << "Time";
    out << '\n';
    for (auto &row : results)
    {
        for (auto col : row)
            out << col << ',';
        out << '\n';
    }
    return 0;
}