#pragma once
#include <crow/json.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <optional>

namespace secsys_server
{
    struct server_config
    {
        struct motor_config
        {
            std::string type;
        };
        std::string mqtt_host;
        int         mqtt_port;
        std::string mqtt_topic;
        std::string pqxx_connection_string;
        std::unordered_map <std::string, motor_config> motors;
        int         http_port;

        static std::optional<server_config> from_file(const std::string& filename);
    };
}