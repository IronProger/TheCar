//
// Created by rege on 09.03.17.
//

#include <kaguya.hpp>

#include "Config.hpp"

using namespace std;

// state of lua script config
kaguya::State state;

Config::Config (string configPath)
{
    state.dofile(configPath);
}

string Config::filePath = DEFAULT_CONFIG_PATH;

void Config::setFilePath (string s)
{
    filePath = s;
}

int Config::getInt (string varName)
{
    return state[varName];
}

bool Config::getBool (string varName)
{
    return state[varName];
}

string Config::getString (string varName)
{
    return state[varName];
}