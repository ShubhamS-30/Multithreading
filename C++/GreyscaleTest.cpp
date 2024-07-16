#include <iostream>
#include <thread>
//#include <dirent.h>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <windows.h>
#include <opencv2/opencv.hpp>
using namespace std;
void ConvertToGreyScale(string sourceImgPath)
{
    cv :: Mat InputImage = cv :: imread(sourceImgPath);

    if (!InputImage.data) {
        cerr<<"Image = "<< sourceImgPath <<"Not found"<<endl;
        return ;
    }
    cv::Mat GreyImage; 
    cv::cvtColor(InputImage, GreyImage, cv::COLOR_BGR2GRAY);
    cv::imshow("Original Image", InputImage);
    cv::imshow("Greyscale Image", GreyImage);

    // Save the greyscale image
    //cv::imwrite("greyscale_image.jpg", greyImage);

    // Wait for a keystroke in the window
    cv::waitKey(0);
}
int main()
{
    string imgPath = "lenna.jpg";
    cout<<"converting To greyscale"<<endl;
    ConvertToGreyScale(imgPath);
    return 0;
}
/*
mkdir Build
cmake -B .\Build\
cmake --build .\Build\ 
.\Build\Debug\MULTITHREADING.exe
*/