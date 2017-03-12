//
// Created by rege on 10.03.17.
//

#include <serial/serial.h>
#include <iostream>
#include "RoboControl.hpp"
#include "Config.hpp"

serial::Serial * serialOut1;
bool fakeSerial;

void init ()
{
    fakeSerial = CGET_BOOL("FAKE_SERIAL");
    if (!fakeSerial) serialOut1 = new serial::Serial(CGET_STR("SERIAL_PORT"), 9600);
}

RoboControl::RoboControl ()
{
    init();
}

void RoboControl::sendStrViaSerial (std::string s)
{
    if (fakeSerial) cout << s << endl;
    else serialOut1->write((const uint8_t *) s.data(), s.size());
}