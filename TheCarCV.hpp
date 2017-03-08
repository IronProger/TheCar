//
// Created by rege on 08.03.17.
//

#ifndef THECAR_THECARCV_HPP
#define THECAR_THECARCV_HPP

#include <functional>

//TODO: this enum to sign detect hpp
typedef enum RoadSign
{ ONLY_FORWARD, ONLY_RIGHT, ONLY_LEFT, ONLY_FORWARD_AND_RIGHT, ONLY_FORWARD_AND_LEFT,
TRAFFIC_LIGHT, STOP, WAY_IS_BUNNED}
        RoadSign;

const int RESOLUTION_OF_IMAGE_FOR_DETECTION = 20;

class OnItemFoundListener
{
    /** mat is a resolution-fixed black-white image for item detection */
public: int operator ()(
            int mat[RESOLUTION_OF_IMAGE_FOR_DETECTION][RESOLUTION_OF_IMAGE_FOR_DETECTION], int size
    ) { };
//protected: void accept(int mat[RESOLUTION_OF_IMAGE_FOR_DETECTION][RESOLUTION_OF_IMAGE_FOR_DETECTION], int size);
};
//public: int operator ()(
//            int mat[RESOLUTION_OF_IMAGE_FOR_DETECTION][RESOLUTION_OF_IMAGE_FOR_DETECTION], int size
//    ) const { };
//};

class TheCarCV
{
protected:
    OnItemFoundListener onItemFoundListener;

    int init();
    void run();

public:
    TheCarCV();

    // warning: this method block the thread (road sign detection start)
    // thread must be unlocked only at program shutdown
    void setOnItemFoundListener(OnItemFoundListener onItemFoundListener);
};

#endif //THECAR_THECARCV_HPP
