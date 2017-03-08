//
// Created by rege on 08.03.17.
//

#include <string>
#include <vector>

#include <plog/Log.h>
#include <opencv2/opencv.hpp>

#include "TheCarCV.hpp"

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

int TheCarCV::init ()
{
    LOGD << "TheCarCV init…";
    // creating windows
    for (string & winName : windows)
    {
        cv::namedWindow(winName, CV_WINDOW_AUTOSIZE);
    }

    //cv::VideoCapture cap(0); //capture the video from web cam

    if (!cap.isOpened())  // if not success, exit program with error
    {
        LOGE << "Cannot open the web cam";
        __throw_system_error(2);
    }

    //Create trackbars in "control" window
    cv::createTrackbar("LowH", "color filter", &iLowH, 179); //Hue (0 - 179)
    cv::createTrackbar("HighH", "color filter", &iHighH, 179);

    cv::createTrackbar("LowS", "color filter", &iLowS, 255); //Saturation (0 - 255)
    cv::createTrackbar("HighS", "color filter", &iHighS, 255);

    cv::createTrackbar("LowV", "color filter", &iLowV, 255); //Value (0 - 255)
    cv::createTrackbar("HighV", "color filter", &iHighV, 255);
}

void TheCarCV::setOnItemFoundListener (OnItemFoundListener onItemFoundListener)
{
    this->onItemFoundListener = onItemFoundListener;
    this->run();
    LOGI << "TheCarCV terminated";
}

void TheCarCV::run ()
{
    for (;;)
    {
        cv::Mat frame;
        if (!cap.read(frame))
        {
            LOGE << "Cannot open the web cam";
            __throw_system_error(2);
        }
        if (cv::waitKey(10) == 27)
        {
            break;
        }
        processFrame(frame);
    }
}

void filterFindRedAndBlue(cv::Mat& src, cv::Mat& dsc)
{
    cv::Mat imgHSV;

    cv::cvtColor(src, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    cv::Mat imgThresholded;

    cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

    //morphological opening (remove small objects from the foreground)
    cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
    cv::dilate( imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    cv::dilate( imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
    cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );

    dsc = imgThresholded;
}

void processFrame (cv::Mat frame)
{
    cv::imshow("original", frame);

    cv::Mat fRAndBInWAndBAnother; // filtered: red and blue to white, and another colors to black
    filterFindRedAndBlue(frame, fRAndBInWAndBAnother);

    cv::imshow("color filter", fRAndBInWAndBAnother);
}