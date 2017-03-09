#include <iostream>

#include <plog/Log.h>

#include "TheCarCV.hpp"
#include "Config.hpp"

using namespace std;

class Listener : public OnItemFoundListener
{
// So hard because I don't fully understood c++ lambdas or because c++ lambdas is so hard.
// If somebody know how to make it to “so easy” — make it.
public:
    void operator() (
            int mat[RESOLUTION_OF_IMAGE_FOR_DETECTION][RESOLUTION_OF_IMAGE_FOR_DETECTION], int size
    )
    {
        //LOGD << "matrix put";
    }
};

int main (int arg, char ** argv)
{
    plog::init(plog::verbose, "log.txt");
    LOGD << "started";

    //Config::filePath = "~/.TheCarConfig.lua";

    Listener listener;
    TheCarCV::getInstance().turnOnWindows();
    TheCarCV::getInstance().start(listener);
}