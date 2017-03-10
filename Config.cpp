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

#if false

int Config::getInt (string varName) throw(NotFoundException)
{
    state("__t__ = type("+varName+")");
    string realVarType = state["__t__"];
    if (realVarType == "nil") throw NotFoundException();
    assert(state["__t__"] == "number");
    return state[varName];
}

bool Config::getBool (string varName) throw(NotFoundException)
{
    state("__t__ = type("+varName+")");
    string realVarType = state["__t__"];
    if (realVarType == "nil") throw NotFoundException();
    assert(state["__t__"] == "boolean");
    return state[varName];
}

string Config::getString (string varName) throw(NotFoundException)
{
    state("__t__ = type("+varName+")");
    string realVarType = state["__t__"];
    if (realVarType == "nil") throw NotFoundException();
    assert(state["__t__"] == "string");
    return state[varName];
}

#endif

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