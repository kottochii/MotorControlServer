#pragma once
#include <crow/http_request.h>
#include <crow/http_response.h>

namespace secsys_server
{
    class server_controller;
    namespace auth_routing
    {
        void route();
        crow::response auth_with_pin(const crow::request& req, int app_id, const std::string& app_public_key, const std::string& pin);
        crow::response auth_with_card(const crow::request& req, int app_id, const std::string& app_public_key, const std::string& id, const std::string& data);
        crow::response auth_with_card_id_only(const crow::request& req, int app_id, const std::string& app_public_key, const std::string& id);
    }
}