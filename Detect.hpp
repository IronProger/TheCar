//
// Created by rege on 13.03.17.
//

#ifndef THECAR_NN_HPP
#define THECAR_NN_HPP

#include <vector>
#include <map>

#include "ImageVector.hpp"

const int MAX_COUNT_OF_CHANGES_FOR_DETECTION = 1000;

enum RoadSignType {
    ONLY_FORWARD, ONLY_RIGHT, ONLY_LEFT, ONLY_RIGHT_OR_FORWARD, ONLY_FORWARD_AND_LEFT,
    TRAFFIC_LIGHT, STOP, WAY_IS_BANNED, UNKNOWN
};

class Detect {
private:
    std::map<RoadSignType, ImageVector> *correct;

    Detect();

    Detect(const Detect &);

    Detect &operator=(Detect &);

    void init();

    double compare(ImageVector &s, ImageVector &x);

public:

    static Detect &getInstance() {
        static Detect instance;
        return instance;
    }

    RoadSignType detect(cv::Mat monochromeImage);
};


#endif //THECAR_NN_HPP
