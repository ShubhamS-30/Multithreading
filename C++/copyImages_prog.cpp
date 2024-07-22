#include <bits/stdc++.h>
#include <thread>
#include <dirent.h>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <windows.h>
using namespace std;

class CopyFiles
{
private:
    string targetFolder = "";
    string file_name_path = "";
    vector<string> imageNames;
    string destinationFolderPath = "";
    bool folderCreated = false;

public:
    CopyFiles(string targetFolder, string file_name_path, string destinationFolderPath)
    {
        this->targetFolder = targetFolder;
        this->file_name_path = file_name_path;
        this->destinationFolderPath = destinationFolderPath;
        GetImageNames();
        createFolder(destinationFolderPath);
    }
    /// @brief Function that stores all the file names in imageNames vector that are stored in a txt file at file_name_path location.
    void GetImageNames()
    {
        ifstream file_names(file_name_path);
        if (!file_names.is_open())
        {
            cerr << "Error opening the file!" << endl;
            return;
        }
        string line;
        while (getline(file_names, line))
        {
            imageNames.push_back(line);
        }
        file_names.close();
    }
    /// @brief Function to create a directory
    /// @param folderName name of the directory to be created
    /// @return true if success else return false
    bool createFolder(const string &folderName)
    {
        // Convert string to LPCSTR (const char*) for Windows API
        LPCSTR path = folderName.c_str();

        // Create directory using CreateDirectory function
        if (CreateDirectory(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
        {
            cout << "Folder " << folderName << " created successfully." << endl;
            folderCreated = true;
            return true;
        }
        else
        {
            cerr << "Failed to create folder. Error code: " << GetLastError() << std::endl;
            return false;
        }
    }

    void Task(int l, int r)
    {
        r = min(r, (int)imageNames.size() - 1);
        string dest_file, source_file;
        for (int i = l; i <= r; i++)
        {
            // destination folder + file name give the relative path to the location where copied files are stored.
            dest_file = "./" + destinationFolderPath + '/' + imageNames[i];
            source_file = targetFolder + imageNames[i];
            // copies file from source destination to target destination.
            CopyFile(source_file.c_str(), dest_file.c_str(), true);
        }
    }
    /// @brief Function to validate the parameters and paths
    /// @param numOfThreads number of threads
    /// @param numOfImages number of images
    /// @return true if everything is fine else false
    bool check(int numOfThreads, int numOfImages)
    {
        if (!folderCreated || numOfImages <= 0 || numOfThreads <= 0 || targetFolder == "" || folderCreated == false || file_name_path == "" || destinationFolderPath == "" || imageNames.empty())
        {
            cout << "ERROR Copying Images" << endl;
            return false;
        }
        else
            return true;
    }
    double CopyImagesUsingMultithreading(int numOfThreads, int numOfImages)
    {
        if (!check(numOfThreads, numOfImages))
            return -1;
        // starting index
        int l = 0;
        // number of images that each thread must process
        int ImagesPerThread = numOfImages / numOfThreads;

        // remainder is distributed between threads
        int rem = numOfImages % numOfThreads;

        vector<thread> threads;
        CopyFiles *instance = this;

        auto begin = chrono::high_resolution_clock::now();

        for (int i = 0; i < numOfThreads; i++)
        {
            if (l == numOfImages)
                break;

            threads.push_back(thread([instance, i, l, ImagesPerThread, rem]()
                                     { instance->Task(l, l + ImagesPerThread + (rem > 0) - 1); }));

            // Next starting point
            l += ImagesPerThread + (rem > 0);

            if (rem > 0)
                rem--;
        }

        // waiting for all threads to finish.
        for (auto &thread : threads)
        {
            thread.join();
        }
        // stoping the timer.
        auto end = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        // All threads have finished executing.
        // return time in seconds.
        return elapsed.count() * 1e-9;
    }
};
/// @brief stores result in csv file
/// @param results contains the time taken
/// @param csv_name name of csv file
void saveResults(vector<vector<double>> &results, string csv_name)
{
    // storing results into csv file
    ofstream out(csv_name.c_str());
    out << "Threads" << ',' << "Time\n";
    for (auto &row : results)
    {
        for (auto col : row)
            out << col << ',';
        out << '\n';
    }
}
int main()
{
    string target_folder = "../IMG/";
    string file_names_path = "../file_names.txt";
    int numOfImages = 10000;
    int threadLimit = 11;
    vector<vector<double>> timeTaken(threadLimit);
    for (int threads = 1; threads <= threadLimit; threads++)
    {
        string folder_name = "CopyImage_" + to_string(threads);
        CopyFiles Copy(target_folder, file_names_path, folder_name);
        double duration = Copy.CopyImagesUsingMultithreading(threads, numOfImages);
        cout << "TIME TAKEN BY THREAD " << threads << " = " << duration << endl;
        timeTaken[threads - 1] = {(double)threads, duration};
    }
    string csv_name = "copyImages_cpp.csv";
    saveResults(timeTaken, csv_name);
    return 0;
}