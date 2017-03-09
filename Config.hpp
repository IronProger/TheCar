//
// Created by rege on 09.03.17.
//

#ifndef THECAR_CONFIG_HPP
#define THECAR_CONFIG_HPP

#include <string>

using namespace std;

const string DEFAULT_CONFIG_PATH = "/etc/TheCarConfig.lua";

typedef class NotFoundException : public exception
{
} NotFoundException;

class Config
{
private:

    Config (string configPath) throw(NotFoundException);

    Config (const Config &) throw(NotFoundException);

    Config & operator= (Config &);

public:
    static string filePath;

    static Config & getInstance ()
    {
        if (filePath == "")
        {
            filePath = DEFAULT_CONFIG_PATH;
        }
        static Config instance(filePath);
        return instance;
    }

    int getInt (string) throw(NotFoundException);

    bool getBool (string) throw(NotFoundException);

    string getString (string) throw(NotFoundException);
};

#endif //THECAR_CONFIG_HPP
