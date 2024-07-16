#include <iostream>
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

public:
    
    void ConvertToGreyScale(string sourceImgPath)
    {
        cv::Mat InputImage = cv::imread(sourceImgPath);

        if (!InputImage.data)
        {
            cerr << "Image = " << sourceImgPath << "Not found" << endl;
            return;
        }
        cv::Mat GreyImage;
        cv::cvtColor(InputImage, GreyImage, cv::COLOR_BGR2GRAY);
        cv::imshow("Original Image", InputImage);
        cv::imshow("Greyscale Image", GreyImage);
        cv::imwrite("greyscale_" + sourceImgPath, GreyImage);
        cv::waitKey(0);
    }
};

int main()
{
    string imgPath = "lenna.jpg";
    cout << "CONVERTING TO GREYSCALE" << endl;
    GreyScaleConverter g;
    g.ConvertToGreyScale(imgPath);
    return 0;
}