//
// Created by rege on 13.03.17.
//
#include <boost/numeric/ublas/matrix.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <plog/Log.h>
#include <map>

#include "Hopfield.hpp"
#include "Detect.hpp"
#include "maps.hpp"

using namespace std;

Hopfield hopfield(50, 50);

Detect::Detect ()
{
    init();
}

void Detect::init ()
{
    LOGD << "Detect using singleton init";

    *signs = create_map<RoadSignType, cv::Mat>
            (ONLY_FORWARD, cv::imread("correct/ONLY_FORWARD.jpg", 0))
            (ONLY_RIGHT, cv::imread("correct/ONLY_RIGHT.jpg", 0))
            (ONLY_LEFT, cv::imread("correct/ONLY_LEFT.jpg", 0))
            (ONLY_RIGHT_OR_FORWARD, cv::imread("correct/ONLY_RIGHT_OR_FORWARD.jpg", 0))
            // where is traffic light? — in dream future only
            (STOP, cv::imread("correct/STOP.jpg", 0))
            (WAY_IS_BUNNED, cv::imread("correct/WAY_IS_BANNED.jpg", 0));

    for (auto && kv : *signs)
    {
        hopfield.addPattern(kv.second);
    }

//     hopfield->calculateWeightsStorkey();
    hopfield.calculateWeightsHebbian();
}

RoadSignType Detect::detect (cv::Mat monochromeImage)
{
    cv::Mat result;
    if (!hopfield.compare(monochromeImage, result, MAX_COUNT_OF_CHANGES_FOR_DETECTION))
    {
        return UNKNOWN;
    }

    for (auto && kv : *signs)
    {
        cv::Mat xorEd;
        cv::bitwise_xor(kv.second, result, xorEd);
        if (cv::countNonZero(xorEd) == 0)
        {
            return kv.first;
        }
    }
    LOGE << "sign in not UNKNOWN and cannot be found in map. I return UNKNOWN";
    return UNKNOWN;
}