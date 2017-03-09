//
// Created by rege on 08.03.17.
//

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <plog/Log.h>

#include "TheCarCV.hpp"
#include "Config.hpp"

using namespace std;

vector<string> windows{"original", "color filter"}; // names for windows
cv::VideoCapture cap(0); //capture the video from web cam

// for color filtering control
int iLowH = 0;
int iHighH = 179;
int iLowS = 0;
int iHighS = 255;
int iLowV = 0;
int iHighV = 255;

void processFrame (cv::Mat frame);

TheCarCV::TheCarCV ()
{
    init();
}

bool TheCarCV::isShowingWindows ()
{
    return this->createWindows;
}

void TheCarCV::turnOnWindows ()
{
    this->createWindows = true;

    /// creating windows

    for (string & winName : windows)
    {
        cv::namedWindow(winName, CV_WINDOW_AUTOSIZE);
    }

    /// "control" window set

    cv::createTrackbar("LowH", "color filter", &iLowH, 179); //Hue (0 - 179)
    cv::createTrackbar("HighH", "color filter", &iHighH, 179);

    cv::createTrackbar("LowS", "color filter", &iLowS, 255); //Saturation (0 - 255)
    cv::createTrackbar("HighS", "color filter", &iHighS, 255);

    cv::createTrackbar("LowV", "color filter", &iLowV, 255); //Value (0 - 255)
    cv::createTrackbar("HighV", "color filter", &iHighV, 255);
}

void TheCarCV::init ()
{
    LOGD << "TheCarCV init…";

    /// video input init

    //cv::VideoCapture cap(0); //capture the video from web cam

    if (!cap.isOpened())  // if not success, exit program with error
    {
        LOGE << "Cannot open the web cam";
        __throw_system_error(2);
    }

    /// configuration set

    iLowH = Config::getInstance().getInt("I_LOW_H");
    iHighH = Config::getInstance().getInt("I_HIGH_H");
    iLowS = Config::getInstance().getInt("I_LOW_S");
    iHighS = Config::getInstance().getInt("I_HIGH_S");
    iLowV = Config::getInstance().getInt("I_LOW_V");
    iHighV = Config::getInstance().getInt("I_HIGH_V");
}

void TheCarCV::start (OnItemFoundListener onItemFoundListener)
{

    for (;;)
    {
        cv::Mat frame;
        if (!cap.read(frame))
        {
            //LOGE << "Cannot open the web cam";
            __throw_system_error(2);
        }
        if (cv::waitKey(10) == 27)
        {
            break;
        }
        processFrame(frame);
    }

    LOGI << "TheCarCV terminated";
}

void filterFindRedAndBlue (cv::Mat & src, cv::Mat & dsc)
{
    cv::Mat imgHSV;

    cv::cvtColor(src, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    cv::Mat imgThresholded;

    cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV),
                imgThresholded); //Threshold the image

    //morphological opening (remove small objects from the foreground)
    cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    //morphological closing (fill small holes in the foreground)
    cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    dsc = imgThresholded;
}

void processFrame (cv::Mat frame)
{
    WIN cv::imshow("original", frame);

    cv::Mat fRAndBInWAndBAnother; // filtered: red and blue to white, and another colors to black
    filterFindRedAndBlue(frame, fRAndBInWAndBAnother);

    WIN cv::imshow("color filter", fRAndBInWAndBAnother);
}