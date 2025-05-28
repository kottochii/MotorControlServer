#pragma once
#include <crow.h>

namespace secsys_server
{
    class server_controller;
    namespace maintenance_routing
    {
        void route();
        crow::response hello(const crow::request& req, int app_id, const std::string& app_public_key);
    };
}