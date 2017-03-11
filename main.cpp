#include <iostream>

#include <plog/Log.h>

#include "TheCarCV.hpp"
#include "Config.hpp"
#include "RoboControl.hpp"

using namespace std;

int main (int arg, char ** argv)
{
    /// init log

    plog::init(plog::verbose, "log.txt");
    LOGD << "started";

    /// init configuration

    Config::setFilePath("TheCarConfig.lua");

    /// conf TheCarCV and start it

    if (CGET_BOOL("SHOW_WINDOWS"))
    {
        LOGI << "windows showing is enabled";
        TheCarCV::getInstance().turnOnWindows();
    }

    TheCarCV::getInstance().start(
            [] (RoadSignData data)
            {
                LOGD << "some sign item put";
                RoboControl::getInstance().sendStrViaSerial("1");
            }
    );

    return 0;
}