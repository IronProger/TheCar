//
// Created by rege on 09.03.17.
//

#ifndef THECAR_CONFIG_HPP
#define THECAR_CONFIG_HPP

#define CGET_INT(s) Config::getInstance().getInt(s)
#define CGET_STR(s) Config::getInstance().getString(s)
#define CGET_BOOL(s) Config::getInstance().getBool(s)

#include <string>

using namespace std;

const string DEFAULT_CONFIG_PATH = "/etc/TheCarConfig.lua";

#if false
typedef class NotFoundException : public exception
{
} NotFoundException;
#endif

// singleton
class Config
{
private:
    static string filePath;

    Config (string configPath);

    Config (const Config &);

    Config & operator= (Config &);

public:
    static Config & getInstance ()
    {
        if (filePath == "")
        {
            filePath = DEFAULT_CONFIG_PATH;
        }
        static Config instance(filePath);
        return instance;
    }

    static void setFilePath (string s);

    int getInt (string);

    bool getBool (string);

    string getString (string);
};

#endif //THECAR_CONFIG_HPP
