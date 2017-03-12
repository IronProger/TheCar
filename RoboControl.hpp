//
// Created by rege on 10.03.17.
//

#ifndef THECAR_ROBOCONTROL_HPP
#define THECAR_ROBOCONTROL_HPP

#include <string>

// singleton
class RoboControl
{
private:
    RoboControl ();

    RoboControl (const RoboControl &);

    RoboControl & operator= (RoboControl &);

public:
    static RoboControl & getInstance ()
    {
        static RoboControl instance;
        return instance;
    }

    void sendStrViaSerial (std::string s);
};


#endif //THECAR_ROBOCONTROL_HPP
