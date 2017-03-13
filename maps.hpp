//
// Created by rege on 13.03.17.
//

#ifndef THECAR_MAPS_HPP
#define THECAR_MAPS_HPP

#include <map>

/// code from: http://stackoverflow.com/questions/138600/initializing-a-static-stdmapint-int-in-c#138612

template<typename T, typename U>
class create_map
{
private:
    std::map<T, U> m_map;
public:
    create_map (const T & key, const U & val)
    {
        m_map[key] = val;
    }

    create_map<T, U> & operator() (const T & key, const U & val)
    {
        m_map[key] = val;
        return *this;
    }

    operator std::map<T, U> ()
    {
        return m_map;
    }
};

#endif //THECAR_MAPS_HPP
