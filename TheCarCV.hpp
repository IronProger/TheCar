//
// Created by rege on 08.03.17.
//

#ifndef THECAR_THECARCV_HPP
#define THECAR_THECARCV_HPP

#include <functional>

#define WIN if (TheCarCV::getInstance().isShowingWindows())

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
};

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
    void start (OnItemFoundListener onItemFoundListener);
};

#endif //THECAR_THECARCV_HPP
