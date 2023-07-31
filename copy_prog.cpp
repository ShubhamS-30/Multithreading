#include <bits/stdc++.h>
#include <thread>
#include <dirent.h>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <windows.h>
using namespace std;
// TASK FUNCTION

// this function takes the parameter 'source_files' that contains the name of all the files to be copied.

// parameters 'l' and 'r' denote the index of elements of 'source_files' from which the copying process starts.

// parameter 'destinatio_folder' contains the path where all copied files are stored.

// parameter 'target_folder' contains the name of folder where all the files to be copied are stored.
void task(int l, int r, string target_folder, vector<string> source_files, string destination_folder)
{
    string target_file, dest_file;
    for (int i = l; i <= r; i++)
    {
        // target folder + file name gives the relative path of file to be copied.
        target_file = target_folder + source_files[i];

        // destination folder + file name give the relative path to the location where copied files are stored.
        dest_file = destination_folder + source_files[i];

        // copies file from source destination to target destination.
        CopyFile(target_file.c_str(), dest_file.c_str(), true);
    }
}
float Multithreading(int num_threads, int file_num, vector<string> &source_files, string target_folder, string destination_folder)
{
    // creates the destination folder
    if (mkdir(destination_folder.c_str()) == 1)
    {
        cout << "FAILED TO CREATE DESTINATION FOLDER" << endl;
        return -1;
    }

    int l, r, batch_size;
    l = 0;
    r = min(file_num, int(source_files.size()));

    // batch size is number of files per thread.
    batch_size = file_num / num_threads;

    // batch size = 0 would cause infinite loop.
    if (batch_size == 0)
    {
        cout << "Number of Images should be greater than the number of threads created" << endl;
        return -1;
    }

    int n = num_threads;

    // timer starts
    auto begin = std::chrono::high_resolution_clock::now();

    // creating n threads
    vector<thread> threads;
    for (int i = 0; i < n; ++i)
    {
        if (i < n - 1)
            threads.push_back(thread(task, l, l + batch_size - 1, target_folder, source_files, destination_folder));
        else
            threads.push_back(thread(task, l, r - 1, target_folder, source_files, destination_folder));

        // incrementing the pointer
        l += batch_size;
    }

    // waiting for all threads to finish.
    for (auto &thread : threads)
    {
        thread.join();
    }
    // stoping the timer.
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    // All threads have finished executing.

    // return time in seconds.
    return elapsed.count() * 1e-9;
}
vector<string> getFiles(string target_folder)
{
    vector<string> source_files;
    // storing the names of all files to be copied.
    DIR *dr;
    struct dirent *en;
    string file;
    // opening target directory
    dr = opendir(target_folder.c_str()); // open all directory
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            file = en->d_name;
            // checking if file name is valid
            if (file.size() > 3 and file.substr(file.size() - 3) == "jpg")
                source_files.push_back(file);
        }
        closedir(dr); // closing the directory
    }
    cout << "Number of files in source folder = " << source_files.size() << endl;
    return source_files;
}
void saveResults(vector<vector<float>> results, string csv_name)
{
    // storing results into csv file
    std::ofstream out(csv_name.c_str());
    out << "Threads" << ',' << "Time";
    out << '\n';
    for (auto &row : results)
    {
        for (auto col : row)
            out << col << ',';
        out << '\n';
    }
}
int main()
{

    // location of folder where files to be copied are stored.
    string target_folder = "./IMG/";

    // geting files to be copied.
    vector<string> source_files = getFiles(target_folder);

    // stores the name of destination folder where copied files are stored.
    string destination_folder;

    // stores time for each iteration
    vector<vector<float>> results;

    // for loop to call multithreading function with i threads.
    for (int i = 1; i <= 11; i++)
    {
        // name of destination folder where copied files are stored.
        destination_folder = "./destination_" + to_string(i) + '/';

        // calling multithreading function for i threads and 10000 files
        float x = Multithreading(i, 10000, source_files, target_folder, destination_folder);

        // adding time taken to results
        results.push_back({float(i), x});
    }

    // printing results
    for (auto x : results)
    {
        cout << x[0] << " " << x[1] << endl;
    }

    // save results into csv file.
    saveResults(results,"copy_images_cpp.csv");
    return 0;
}