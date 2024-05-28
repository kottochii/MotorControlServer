#pragma once
#include "models/user_model.hpp"
#include <optional>
#include <pqxx/pqxx>
#include <string>

namespace secsys_server
{
    class server_controller;
    class authorisation;
    class user_controller
    {
        server_controller& _controller;
        public:
        user_controller(server_controller& server_controller);
        std::optional<user> get_by_id(uint32_t id);
        std::optional<user> get_by_pin(const std::string& pin);
        std::optional<user> get_by_card(const std::string& card_id, const std::string& data);
        std::optional<std::string> create_card(const std::string& card_id, uint32_t id);
        //std::optional<user> get_by_auth(const authorisation& auth);
        //bool commit(user& user);
        static std::optional<user> user_from_result(const pqxx::result& res);
    };
}