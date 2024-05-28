#pragma once
#include <stdexcept>

namespace secsys_server
{
    class mqtt_connection_failed : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };
}