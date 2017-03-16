//
// Created by user on 15.03.17.
//

#include <opencv2/highgui.hpp>
#include <plog/Log.h>

#include "Detect.hpp"

using namespace std;

void Detect::init() {
    // load all data
    const cv::Mat onlyForward = cv::imread("correct/ONLY_FORWARD.jpg", 0);
    const cv::Mat onlyRight = cv::imread("correct/ONLY_RIGHT.jpg", 0);
    const cv::Mat onlyLeft = cv::imread("correct/ONLY_LEFT.jpg", 0);
    const cv::Mat onlyRightOrForward = cv::imread("correct/ONLY_RIGHT_OR_FORWARD.jpg", 0);
    const cv::Mat wayIsBanned = cv::imread("correct/WAY_IS_BANNED.jpg", 0);
    const cv::Mat stop = cv::imread("correct/STOP.jpg", 0);

    correct = new std::map<RoadSignType, ImageVector>;
    (*correct)[ONLY_FORWARD] = ImageVector(std::vector<cv::Mat>{
            cv::imread("correct/ONLY_FORWARD_0.jpg"),
            cv::imread("correct/ONLY_FORWARD_1.jpg"),
            cv::imread("correct/ONLY_FORWARD_2.jpg"),
            cv::imread("correct/ONLY_FORWARD_3.jpg"),
            cv::imread("correct/ONLY_FORWARD_4.jpg")
    });
    (*correct)[ONLY_RIGHT] = ImageVector(cv::imread("correct/ONLY_RIGHT.jpg", 0));
    (*correct)[ONLY_LEFT] = ImageVector(cv::imread("correct/ONLY_LEFT.jpg", 0));
    (*correct)[ONLY_RIGHT_OR_FORWARD] = ImageVector(cv::imread("correct/ONLY_RIGHT_OR_FORWARD.jpg", 0));
    (*correct)[WAY_IS_BANNED] = ImageVector(std::vector<cv::Mat>{
            cv::imread("correct/WAY_IS_BANNED_0.jpg"),
            cv::imread("correct/WAY_IS_BANNED_1.jpg"),
            cv::imread("correct/WAY_IS_BANNED_2.jpg"),
            cv::imread("correct/WAY_IS_BANNED_3.jpg"),
            cv::imread("correct/WAY_IS_BANNED_4.jpg")
    });
    (*correct)[STOP] = ImageVector(cv::imread("correct/STOP.jpg", 0));
#if false
    *correct = create_map<RoadSignType, ImageVector>
            (ONLY_FORWARD, ImageVector(
                    onlyForward
            ))
            (ONLY_RIGHT, ImageVector(
                    onlyRight
            ))
            (ONLY_LEFT, ImageVector(
                    onlyLeft
            ))
            (ONLY_RIGHT_OR_FORWARD, ImageVector(
                    onlyRightOrForward
            ))
            // where is traffic light? — in dream future only
            (WAY_IS_BANNED, ImageVector(
                    wayIsBanned
            ))
            (STOP, ImageVector(
                    stop
            ));
#endif
}

double Detect::compare(ImageVector &s, ImageVector &x) {
    assert(s.getSize() == x.getSize());
    ulong size = s.getSize();
    double sum = 0;
    for (ulong i = 0; i < size; i++) {
        sum += (s.get(i) * x.get(i));
    }

    double sLen = s.vectorLen(), xLen = x.vectorLen();
    double posit = sLen * xLen;
    double toAcos = sum / posit;
    double result = acos(toAcos);
    if (result == NAN) LOGE << "NAN !!!";
    return result;
}

RoadSignType Detect::detect(cv::Mat monochromeImage) {

    double minAngle = 100; // 100 is impossible value, because 100
    RoadSignType minAngleSign = UNKNOWN;

    ImageVector iv(monochromeImage);

    for (auto &&kv : *correct) {
        double angle = compare(iv, kv.second);
        if (angle < minAngle) {
            minAngle = angle;
            minAngleSign = kv.first;
        }
    }
    LOGD << "min angle: " + to_string(minAngle);

    // image is bed
    //if (minAngle > 0.6) return UNKNOWN;

    return minAngleSign;
}

Detect::Detect() {
    init();
}
