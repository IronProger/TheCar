#include <iostream>

#include <plog/Log.h>

#include "TheCarCV.hpp"
#include "Config.hpp"

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

    TheCarCV::getInstance().start([] (int * matrix[20], int distance)
                                  {
                                      LOGD << "some sign item put";
                                  });

    return 0;
}