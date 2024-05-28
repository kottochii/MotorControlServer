#pragma once
#include <crow.h>

namespace secsys_server
{
    class server_controller;
    namespace user_routing
    {
        void route();
        //crow::response get_all(const crow::request& req);
        crow::response get_by_id(const crow::request& req, uint32_t id);
        crow::response get_by_pin(const crow::request& req, const std::string& pin);
        crow::response register_card(const crow::request& req, const std::string& card_id);
    };
}