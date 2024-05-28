#pragma once
#include <crow.h>

namespace secsys_server
{
    class server_controller;
    namespace motor_routing
    {
        void route();
        crow::response get_all(const crow::request& req);
        crow::response get_by_id(const crow::request& req, const std::string& id);
        crow::response open_by_id(const crow::request& req, const std::string& id);
        crow::response close_by_id(const crow::request& req, const std::string& id);
        crow::response stop_by_id(const crow::request& req, const std::string& id);
    };
}