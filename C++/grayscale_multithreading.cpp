#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;
class GrayScaleConverter
{
private:
    string targetFolder;
    string file_name_path;
    vector<string> imageNames;
    string destinationFolderPath;

public:
    GrayScaleConverter(string targetFolder, string file_name_path, string destinationFolderPath)
    {
        this->targetFolder = targetFolder;
        this->file_name_path = file_name_path;
        this->destinationFolderPath = destinationFolderPath;
        GetImageNames();
        createFolder(this->destinationFolderPath);
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
            cout << "Folder created successfully." << endl;
            return true;
        }
        else
        {
            cerr << "Failed to create folder. Error code: " << GetLastError() << std::endl;
            return false;
        }
    }

    /// @brief function to convert image at sourceImg location to grayscale.
    /// @param sourceImg location of input image.
    void ConvertToGrayScale(string sourceImg)
    {
        string sourceImgPath = targetFolder + sourceImg;
        cv::Mat InputImage = cv::imread(sourceImgPath);

        if (!InputImage.data)
        {
            cerr << "Image = " << sourceImgPath << "Not found" << endl;
            return;
        }
        cv::Mat GrayImage;
        cv::cvtColor(InputImage, GrayImage, cv::COLOR_BGR2GRAY);
        SaveImage(GrayImage, sourceImg);
    }

    /// @brief  Save GrayImg to the destination path with name filename
    /// @param GrayImg Image to be saved
    /// @param filename name of the saved file
    void SaveImage(cv::Mat GrayImg, string filename)
    {
        cv::imwrite("./" + destinationFolderPath + "/grayscale_" + filename, GrayImg);
    }

    /// @brief converts images in imageNames vector from index l to r to grayscale.
    /// @param l starting index
    /// @param r ending index
    void Task(int l, int r)
    {
        r = min(r,(int)imageNames.size()-1);
        for (int i = l; i <= r; i++)
        {
            ConvertToGrayScale(imageNames[i]);
        }
    }

    /// @brief Uses multithreading to convert large number of images to grayscale
    /// @param numOfThreads number of threads to be used for multithreading 
    /// @param numOfImages number of images to be converted to grayscale
    /// @return time take in seconds
    double ConvertToGrayScaleUsingMultithreading(int numOfThreads, int numOfImages)
    {
        // starting index
        int l = 0;
        // number of images that each thread must process
        int ImagesPerThread = numOfImages / numOfThreads;

        // remainder is distributed between threads
        int rem = numOfImages % numOfThreads;

        vector<thread> threads;
        GrayScaleConverter *instance = this;

        auto begin = chrono::high_resolution_clock::now();

        for (int i = 0; i < numOfThreads; i++)
        {
            if(l == numOfImages)
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
    int numOfImages = 30000;
    int threadLimit = 11;
    vector<vector<double>> timeTaken(threadLimit);
    for(int threads = 1;threads<=threadLimit;threads++)
    {
        string folder_name = "Grayscale_" + to_string(threads);
        GrayScaleConverter g(target_folder, file_names_path, folder_name);
        double duration = g.ConvertToGrayScaleUsingMultithreading(threads, numOfImages);
        cout << "TIME TAKEN BY THREAD "<< threads <<" = " <<duration<< endl;

        timeTaken[threads-1] = {(double)threads,duration};
    }
    string csv_name = "grayscale_images_C++.csv";
    saveResults(timeTaken,csv_name);
    
    return 0;
}