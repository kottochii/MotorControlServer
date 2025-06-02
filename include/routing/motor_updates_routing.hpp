#pragma once
#include <crow.h>

namespace secsys_server
{
    class server_controller;
    namespace motor_updates_routing
    {
        void route();

        // status update socket:
        bool status_updates_onaccept(const crow::request& req, void** userdata);
        void status_updates_onopen(crow::websocket::connection& conn);
        // void status_updates_onmessage(crow::websocket::connection& conn, const std::string& message, bool is_binary);
        // void status_updates_onerror(crow::websocket::connection& conn, const std::string& error_message);
        void status_updates_onclose(crow::websocket::connection& conn, const std::string& reason, uint16_t with_status_code);
    }
}