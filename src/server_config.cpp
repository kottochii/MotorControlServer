#include "config/server_config.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>

using namespace secsys_server;
#define GET_PARAM_FROM_CONFIG(param_name_config, param_name_error, destination, original_ptree, T) \
{ \
    auto param_pt = original_ptree.get_child_optional(param_name_config); \
    if(!param_pt.has_value()) \
    { \
        std::cerr << "Config parser error: \"" param_name_error "\" is not defined" << std::endl; \
        return std::nullopt; \
    } \
    /* get the value out of it */ \
    auto param_value = param_pt->get_value_optional<T>(); \
    if(!param_value.has_value()) \
    { \
        std::cerr << "Config parser error: \"" param_name_error "\" is not of correct value" << std::endl; \
        return std::nullopt; \
    } \
    destination = *param_value; \
}


std::optional<server_config> server_config::from_file(const std::string& filename)
{
    std::ifstream fs(filename);
    if(!fs.is_open())
    {
        std::cerr << "Config is not openable at " << filename << std::endl;
        return std::nullopt;
    }

    server_config cfg;
    boost::property_tree::ptree ptree;
    boost::property_tree::read_json(fs, ptree);

    {
        // mqtt
        auto mqtt_pt = ptree.get_child_optional("mqtt");
        if(!mqtt_pt.has_value())
        {
            std::cerr << "Config parser error: \"mqtt\" is not defined" << std::endl;
            return std::nullopt;
        }
        // mqtt::host
        GET_PARAM_FROM_CONFIG("host", "mqtt::host", cfg.mqtt_host, (*mqtt_pt), std::string);
        // mqtt::port
        GET_PARAM_FROM_CONFIG("port", "mqtt::port", cfg.mqtt_port, (*mqtt_pt), int);
        // mqtt::topic
        GET_PARAM_FROM_CONFIG("topic", "mqtt::topic", cfg.mqtt_topic, (*mqtt_pt), std::string);
    }

    // pqxx
    {
        auto pqxx_pt = ptree.get_child_optional("pqxx");
        if(!pqxx_pt.has_value())
        {
            std::cerr << "Config parser error: \"pqxx\" is not defined" << std::endl;
            return std::nullopt;
        }
        // pqxx::connection_string
        GET_PARAM_FROM_CONFIG("connection_string", "pqxx::connection_string", cfg.pqxx_connection_string, (*pqxx_pt), std::string);
    }


    // motors
    {
        auto motors_pt = ptree.get_child_optional("motors");
        if(!motors_pt.has_value())
        {
            std::cerr << "Config parser error: \"motors\" is not defined" << std::endl;
            return std::nullopt;
        }
        for(auto i = motors_pt->begin(); i != motors_pt->end(); i++)
        {
            using namespace std::string_literals;
            // slug of the motor
            std::cout << i->first << std::endl;
            // parse each motor configuration
            motor_config currmotor;
            // since this is a #define, second parameter looks weird but it is not a mistake
            GET_PARAM_FROM_CONFIG("type", "motor::type at motor " << i->first << , currmotor.type, (i->second), std::string);
            cfg.motors.insert({i->first, currmotor});
        }
    }
    // http
    {
        auto http_pt = ptree.get_child_optional("http");
        if(!http_pt.has_value())
        {
            std::cerr << "Config parser error: \"http\" is not defined" << std::endl;
            return std::nullopt;
        }
        // http::port
        GET_PARAM_FROM_CONFIG("port", "http::port", cfg.http_port, (*http_pt), int);
    }

    return cfg;
}