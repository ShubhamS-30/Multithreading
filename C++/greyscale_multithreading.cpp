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
class GreyScaleConverter
{
private:
    string targetFolder;
    string file_name_path;
    vector<string> imageNames;

public:
    GreyScaleConverter(string targetFolder, string file_name_path)
    {
        this->targetFolder = targetFolder;
        this->file_name_path = file_name_path;
        GetImageNames();
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
    /// @brief function to convert image at sourceImg location to greyscale.
    /// @param sourceImg location of input image.
    void ConvertToGreyScale(string sourceImg)
    {
        string sourceImgPath = targetFolder + sourceImg;
        cv::Mat InputImage = cv::imread(sourceImgPath);

        if (!InputImage.data)
        {
            cerr << "Image = " << sourceImgPath << "Not found" << endl;
            return;
        }
        cv::Mat GreyImage;
        cv::cvtColor(InputImage, GreyImage, cv::COLOR_BGR2GRAY);
        // cv::imshow("Original Image", InputImage);
        // cv::imshow("Greyscale Image", GreyImage);
        // cv::waitKey(0);

        // cv::imwrite("greyscale_" + sourceImg, GreyImage);
    }
    float ConvertToGreyScaleUsingMultithreading(int numOfThreads, int numOfImages)
    {
        // TODO:
        // return time taken
        return 0;
    }
};

int main()
{
    string target_folder = "../IMG/";
    string file_names_path = "../file_names.txt";
    GreyScaleConverter g(target_folder, file_names_path);
    g.ConvertToGreyScale("1.jpg");
    return 0;
}