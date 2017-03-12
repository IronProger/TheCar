//
// Created by rege on 08.03.17.
//

#include <plog/Log.h>

#include "TheCarCV.hpp"
#include "Config.hpp"

using namespace std;
using namespace experimental;

/**
 * warning: before every line, which works with windows, macro “IFWIN” must be put
 * (this lines will be skipped when windows mode will be off)
 * */

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
    // (“IFWIN” macro doesn't need for this function) 

    this->createWindows = true;

    /// creating windows

    for (string & winName : *windows)
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

    cv::createTrackbar("dp", "color filter", &hDp, 200);
    cv::createTrackbar("minDist", "color filter", &hMinDist, 200);
    cv::createTrackbar("param1", "color filter", &hParam1, 300);
    cv::createTrackbar("param2", "color filter", &hParam2, 300);
    cv::createTrackbar("minRadius", "color filter", &hMinRadius, 200);
    cv::createTrackbar("maxRadius", "color filter", &hMaxRadius, 200);

    cv::createTrackbar("threshold", "edges", &edgeThreshold, 100);
}

void TheCarCV::init ()
{
    LOGD << "TheCarCV init…";

    /// video output init

    // names for windows, which will be created
    IFWIN windows = new vector<string>{"original", "color filter", "blue", "red", "edges"};

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
    hMinDist = CGET_INT("H_MINDIST");
    hParam1 = CGET_INT("H_PARAM1");
    hParam2 = CGET_INT("H_PARAM2");
    hMinRadius = CGET_INT("H_MINRADIUS");
    hMaxRadius = CGET_INT("H_MAXRADIUS");

    edgeThreshold = CGET_INT("EDGE_THRESHOLD");

    dirForTestImagesOutput = (char *) CGET_STR("FOLDER_FOR_TEST_IMAGES_OUTPUT").c_str();

    /// prepare system

    if (!filesystem::exists(filesystem::path(dirForTestImagesOutput)))
    {
        filesystem::create_directory(filesystem::path(dirForTestImagesOutput));
    }
}

void TheCarCV::start (void (* onItemFoundListener) (RoadSignData))
{
    vector<RoadSignData> roadSigns;

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
        processFrame(frame, roadSigns);

        if (roadSigns.size() > 1) // if some road signs detected, put each sign to a sign listener
        {
            for (RoadSignData & roadSign : roadSigns)
            {
                onItemFoundListener(roadSign);
            }
        }
    }

    LOGI << "TheCarCV terminated";
}

void TheCarCV::hsvFilter (
        cv::Mat & src, cv::Mat & dsc,
        int lh, int hh,
        int ls, int hs,
        int lv, int hv
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

vector<cv::Vec3f> TheCarCV::getCirclesFromMonochrome (cv::Mat blackWhite)
{
    vector<cv::Vec3f> circles;
    HoughCircles(blackWhite, circles, CV_HOUGH_GRADIENT,
                 hDp, // The inverse ratio of resolution
                 hMinDist, // Minimum distance between detected centers
                 hParam1, //60 // Upper threshold for the internal Canny edge detector
                 hParam2, //10 // Threshold for center detection
                 hMinRadius,
                 hMaxRadius); // Maximum and minimum radiuses to be detected. If unknown, put zero as default
    return circles;
}

// resize an image to resolution for detection (which equals const RESOLUTION_OF_IMAGE_FOR_DETECTION)
// if success or changes doesn't needs then return true, if src image resolution smaller than RESOLUTION_OF_IMAGE_FOR_DETECTION then return false
bool TheCarCV::resizeForDetection (cv::Mat & src, cv::Mat & dsc)
{
    if (src.rows == RESOLUTION_OF_IMAGE_FOR_DETECTION && src.cols == RESOLUTION_OF_IMAGE_FOR_DETECTION)
    {
        LOGD << "source image resolution equals with RESOLUTION_OF_IMAGE_FOR_DETECTION, I just simply return it";
        src.copyTo(dsc);
        return true;
    }
    // I don't know what would happen if rows equals with RESOLUTION_OF_IMAGE_FOR_DETECTION
    // and cols higher or vise versa. It case never happen here because it impossible with squares
    if (src.rows < RESOLUTION_OF_IMAGE_FOR_DETECTION || src.cols < RESOLUTION_OF_IMAGE_FOR_DETECTION)
    {
        LOGD << "source image x resolution or y resolution is smaller than RESOLUTION_OF_IMAGE_FOR_DETECTION, I need to return false";
        return false;
    }
    cv::resize(src, dsc, cv::Size(RESOLUTION_OF_IMAGE_FOR_DETECTION, RESOLUTION_OF_IMAGE_FOR_DETECTION));
}

// edges detection with controlable trashhold
void TheCarCV::edgeDetect (cv::Mat & src, cv::Mat & dsc)
{
    cv::Mat srcGray;
    cvtColor(src, srcGray, CV_BGR2GRAY);
    cv::blur(srcGray, srcGray, cv::Size(3, 3));
    int ratio = 3;
    int kernel_size = 3;
    cv::Mat edges;
    cv::Canny(srcGray, edges, edgeThreshold, edgeThreshold * ratio, kernel_size);
    dsc = edges;
}

// if success then return true else return false
// do not resize a result image — it's already resized to RESOLUTION_OF_IMAGE_FOR_DETECTION
bool TheCarCV::cutSquareRegionByCircle (cv::Mat & src, cv::Mat & dsc, int x, int y, int radius)
{
    // first math rect x and y, height and width
    int rectX, rectY, rectHeight, rectWidth;
    rectX = x-radius;
    rectY = y-radius;
    rectHeight = radius * 2;
    rectWidth = radius * 2;

    // then cat an square
    cv::Rect rect(rectX, rectY, rectWidth, rectHeight);

    cv::Mat cutRect;
    if (0 <= rectX && 0 <= rectWidth && rectX+rectWidth <= src.cols
        && 0 <= rectY && 0 <= rectHeight && rectY+rectHeight <= src.rows)
    {
        src(rect).copyTo(cutRect);
    } else
    {
        LOGW << "expression (0 <= rectX && 0 <= rectWidth && rectX + rectWidth <= src.cols && 0 <= rectY && 0 <= rectHeight && rectY + rectHeight <= src.rows) returned false";
        return false;
    }

    return resizeForDetection(cutRect, src);
}

inline bool TheCarCV::cutSquareRegionByCircle (cv::Mat & src, cv::Mat & dsc, cv::Vec3f circle)
{
    return cutSquareRegionByCircle(src, dsc, circle[0], circle[1], circle[2]);
}

void TheCarCV::processFrame (cv::Mat frame, vector<RoadSignData> & roadSigns)
{
    roadSigns.clear();

    // HSV filtering image show special for user experiments
    IFWIN
    {
        cv::Mat colorFilterContr;
        hsvFilter(frame, colorFilterContr, iLowH, iHighH, iLowS, iHighS, iLowV, iHighV);
        cv::imshow("color filter", colorFilterContr);
        vector<cv::Vec3f> circles = getCirclesFromMonochrome(colorFilterContr);
        for (cv::Vec3f & c : circles) // show found circles
        {
            cv::Point center(cvRound(c[0]), cvRound(c[1]));
            cv::circle(frame, center, c[2], cv::Scalar(0, 100, 50), 3, CV_AA, 0);
        }
    }

    // for user experiments too
    cv::Mat edges;
    edgeDetect(frame, edges);
    IFWIN
    {
        cv::imshow("edges", edges);
        vector<cv::Vec3f> circles = getCirclesFromMonochrome(edges);
        for (cv::Vec3f & c : circles) // show found circles
        {
            cv::Point center(cvRound(c[0]), cvRound(c[1]));
            cv::circle(frame, center, c[2], cv::Scalar(0, 50, 150), 3, CV_AA, 0);
        }
    }

    cv::Mat red;
    hsvFilter(frame, red, redILowH, redIHighH, redILowS, redIHighS, redILowV, redIHighV);
    IFWIN cv::imshow("red", red);

    cv::Mat blue;
    hsvFilter(frame, blue, blueILowH, blueIHighH, blueILowS, blueIHighS, blueILowV, blueIHighV);
    IFWIN cv::imshow("blue", blue);

    //tmp code
    {
        auto onCircleFound = [this, &frame, &edges] (cv::Vec3f circle)
        {
            cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
            cv::circle(frame, center, circle[2], cv::Scalar(150, 50, 0), 3, CV_AA, 0);
            static int i;
            cv::Mat square;
            if (!cutSquareRegionByCircle(edges, square, (const cv::Vec3f) circle))
            {
                LOGW << "cutSquareRegionByCircle() function call returned false. Image haven't get and will not saved";
                return;
            }
            cv::imwrite(dirForTestImagesOutput+"/test_output"+to_string(i++)+".jpg", square);
        };
        // tmp code
        RoadSignData roadSignData;
        vector<cv::Vec3f> redCircles = getCirclesFromMonochrome(red);
        for (cv::Vec3f & item : redCircles) onCircleFound(item);
        vector<cv::Vec3f> blueCircles = getCirclesFromMonochrome(blue);
        for (cv::Vec3f & item : blueCircles) onCircleFound(item);
    }

    IFWIN cv::imshow("original", frame);
}