//
// Created by rege on 08.03.17.
//

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <plog/Log.h>
#include <serial/serial.h>

#include "TheCarCV.hpp"
#include "Config.hpp"

using namespace std;

/**
 * warning: before every line, which works with windows, macro “WIN” must be put
 * (this lines will be skipped)
 * */

vector<string> windows{"original", "color filter"}; // names for windows
cv::VideoCapture * cap; //capture the video from web cam

// for color filtering control
int iLowH;
int iHighH;
int iLowS;
int iHighS;
int iLowV;
int iHighV;

int redILowH;
int redIHighH;
int redILowS;
int redIHighS;
int redILowV;
int redIHighV;

int blueILowH;
int blueIHighH;
int blueILowS;
int blueIHighS;
int blueILowV;
int blueIHighV;

// hough circle transform parametrs
int hDp;
int hMindist;
int hParam1;
int hParam2;
int hMinRadius;
int hMaxRadius;

int edgeTrashhold;

serial::Serial * serialOut;

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

    WIN cv::createTrackbar("LowH", "color filter", &iLowH, 179); //Hue (0 - 179)
    WIN cv::createTrackbar("HighH", "color filter", &iHighH, 179);

    WIN cv::createTrackbar("LowS", "color filter", &iLowS, 255); //Saturation (0 - 255)
    WIN cv::createTrackbar("HighS", "color filter", &iHighS, 255);

    WIN cv::createTrackbar("LowV", "color filter", &iLowV, 255); //Value (0 - 255)
    WIN cv::createTrackbar("HighV", "color filter", &iHighV, 255);

    WIN cv::createTrackbar("dp", "color filter", &hDp, 200);
    WIN cv::createTrackbar("mindist", "color filter", &hMindist, 200);
    WIN cv::createTrackbar("param1", "color filter", &hParam1, 300);
    WIN cv::createTrackbar("param2", "color filter", &hParam2, 300);
    WIN cv::createTrackbar("minRadius", "color filter", &hMinRadius, 200);
    WIN cv::createTrackbar("maxRadius", "color filter", &hMaxRadius, 200);

    WIN cv::createTrackbar("edgeTrashhold", "color filter", &edgeTrashhold, 100);
}

void TheCarCV::init ()
{
    LOGD << "TheCarCV init…";

    /// video input init

    int videoInputSourceNumber = CGET_INT("VIDEO_INPUT_SOURCE_NUMBER");
    try
    {
        cap = new cv::VideoCapture(videoInputSourceNumber); //capture the video from web cam
    }
    catch (exception e)
    {
        LOGF << "cannot open webcam "+to_string(videoInputSourceNumber);
        throw (e);
    }
    LOGI << "video input source "+to_string(videoInputSourceNumber)+" has set";

    if (!cap->isOpened())  // if not success, exit program with error
    {
        LOGE << "Cannot open the web cam";
        __throw_system_error(2);
    } else
        LOGI << "video input source "+to_string(videoInputSourceNumber)+" has opened seccesfully";

    /// configuration set

    redILowH = CGET_INT("RED_I_LOW_H");
    redIHighH = CGET_INT("RED_I_HIGH_H");
    redILowS = CGET_INT("RED_I_LOW_S");
    redIHighS = CGET_INT("RED_I_HIGH_S");
    redILowV = CGET_INT("RED_I_LOW_V");
    redIHighV = CGET_INT("RED_I_HIGH_V");

    blueILowH = CGET_INT("BLUE_I_LOW_H");
    blueIHighH = CGET_INT("BLUE_I_HIGH_H");
    blueILowS = CGET_INT("BLUE_I_LOW_S");
    blueIHighS = CGET_INT("BLUE_I_HIGH_S");
    blueILowV = CGET_INT("BLUE_I_LOW_V");
    blueIHighV = CGET_INT("BLUE_I_HIGH_V");

    hDp = CGET_INT("H_DP");
    hMindist = CGET_INT("H_MINDIST");
    hParam1 = CGET_INT("H_PARAM1");
    hParam2 = CGET_INT("H_PARAM2");
    hMinRadius = CGET_INT("H_MINRADIUS");
    hMaxRadius = CGET_INT("H_MAXRADIUS");

    edgeTrashhold = CGET_INT("EDGE_TRASHHOLD");

    /// serial

    serialOut = new serial::Serial("/dev/ttyACM0", 9600);
}

void TheCarCV::start (void (* onItemFoundListener) (int **, int))
{

    for (;;)
    {
        cv::Mat frame;
        if (!cap->read(frame))
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

void filterFindRedAndBlue (
        cv::Mat & src, cv::Mat & dsc,
        int lh = iLowH, int hh = iHighH,
        int ls = iLowS, int hs = iHighS,
        int lv = iLowV, int hv = iHighV
)
{
    cv::Mat imgHSV;

    cv::cvtColor(src, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    cv::Mat imgThresholded;

    cv::inRange(imgHSV, cv::Scalar(lh, ls, lv), cv::Scalar(hh, hs, hv),
                imgThresholded); //Threshold the image

    //morphological opening (remove small objects from the foreground)
    cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    //morphological closing (fill small holes in the foreground)
    cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    dsc = imgThresholded;
}

vector<cv::Vec3f> getCirclerFromBW (cv::Mat blackWhite)
{
    vector<cv::Vec3f> circles;
    HoughCircles(blackWhite, circles, CV_HOUGH_GRADIENT,
                 hDp, // The inverse ratio of resolution
                 hMindist, // Minimum distance between detected centers
                 hParam1, //60 // Upper threshold for the internal Canny edge detector
                 hParam2, //10 // Threshold for center detection
                 hMinRadius,
                 hMaxRadius); // Maximum and minimum radiuses to be detected. If unknown, put zero as default
    return circles;
}

// edges detection with controlable trashhold
void edgeDetect (cv::Mat & src, cv::Mat & dsc)
{
    cv::Mat srcGray;
    cvtColor(src, srcGray, CV_BGR2GRAY);
    cv::Mat edges;
    cv::blur(srcGray, edges, cv::Size(3, 3));
    int ratio = 3;
    int kernel_size = 3;
    cv::Canny(edges, edges, edgeTrashhold, edgeTrashhold * ratio, kernel_size);
    dsc = edges;
}

void processFrame (cv::Mat frame)
{
    cv::Mat colorFilterContr;
    filterFindRedAndBlue(frame, colorFilterContr);
    WIN cv::imshow("color filter", colorFilterContr);
    vector<cv::Vec3f> circles = getCirclerFromBW(colorFilterContr);
    WIN for (cv::Vec3f & c : circles) // show found circles
        {
            cv::Point center(cvRound(c[0]), cvRound(c[1]));
            cv::circle(frame, center, c[2], cv::Scalar(0, 100, 50), 3, CV_AA, 0);
        }
    WIN cv::imshow("original", frame);


    cv::Mat red;
    filterFindRedAndBlue(frame, red, redILowH, redIHighH, redILowS, redIHighS, redILowV, redIHighV);
    WIN cv::imshow("red", red);

    cv::Mat blue;
    filterFindRedAndBlue(frame, blue, blueILowH, blueIHighH, blueILowS, blueIHighS, blueILowV, blueIHighV);
    WIN cv::imshow("blue", blue);

    vector<cv::Vec3f> redCircles = getCirclerFromBW(red);
    for (cv::Vec3f & item : redCircles) serialOut->write(vector<uint8_t>{1});
    vector<cv::Vec3f> blueCircles = getCirclerFromBW(blue);
    for (cv::Vec3f & item : blueCircles) serialOut->write(vector<uint8_t>{1});
}