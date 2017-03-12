//
// Created by rege on 08.03.17.
//

#ifndef THECAR_THECARCV_HPP
#define THECAR_THECARCV_HPP

#include <functional>

#define IFWIN if (TheCarCV::getInstance().isShowingWindows())

//TODO: this enum to sign detect hpp
typedef enum RoadSignType
{ ONLY_FORWARD, ONLY_RIGHT, ONLY_LEFT, ONLY_FORWARD_AND_RIGHT, ONLY_FORWARD_AND_LEFT,
TRAFFIC_LIGHT, STOP, WAY_IS_BUNNED}
        RoadSign;

const int RESOLUTION_OF_IMAGE_FOR_DETECTION = 20;

typedef struct RoadSignData
{
    int mat[RESOLUTION_OF_IMAGE_FOR_DETECTION][RESOLUTION_OF_IMAGE_FOR_DETECTION];
    int distance;
} RoadSignData;

// singleton
class TheCarCV
{
private:
    bool createWindows = false;

    TheCarCV ();

    TheCarCV (const TheCarCV &);

    TheCarCV & operator= (TheCarCV &);

    void init ();

public:

    static TheCarCV & getInstance ()
    {
        static TheCarCV instance;
        return instance;
    }

    // initialize windows and turn on showing of it
    void turnOnWindows ();

    // if windows are showing on screen return true, else false
    bool isShowingWindows ();

    // warning: this method block the thread (road sign detection start)
    // thread must be unlocked only at program shutdown
    void start (void (* onItemFoundListener) (RoadSignData));
};

#endif //THECAR_THECARCV_HPP
