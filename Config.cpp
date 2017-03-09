//
// Created by rege on 09.03.17.
//

#include <kaguya.hpp>

#include "Config.hpp"

using namespace std;

// state of lua script config
kaguya::State state;

Config::Config (string configPath) throw(NotFoundException)
{
    try
    {
        state.dofile(configPath);
    }
    catch (exception e)
    {
        throw NotFoundException();
    }
}

string Config::filePath = DEFAULT_CONFIG_PATH;

int Config::getInt (string varName) throw(NotFoundException)
{
    state("__t__ = type("+varName+")");
    string realVarType = state["__t__"];
    if (realVarType == "nil") throw NotFoundException();
    assert(state["__t__"] == "number");
    return strtol(state[varName], NULL, 10);
}

bool Config::getBool (string varName) throw(NotFoundException)
{
    state("__t__ = type("+varName+")");
    string realVarType = state["__t__"];
    if (realVarType == "nil") throw NotFoundException();
    assert(state["__t__"] == "boolean");
    if (state[varName] == "true") return true; else return false;
}

string Config::getString (string varName) throw(NotFoundException)
{
    state("__t__ = type("+varName+")");
    string realVarType = state["__t__"];
    if (realVarType == "nil") throw NotFoundException();
    assert(state["__t__"] == "string");
    return state[varName];
}