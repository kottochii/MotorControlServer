#pragma once
#include <optional>
#include <crow.h>
#include <pqxx/pqxx>
#include "models/authorisation.hpp"

namespace secsys_server
{
    class server_controller;
    class authorisation_controller
    {
        server_controller& _controller;
        static std::optional<authorisation> from_result(const pqxx::result& result);
        public:
        authorisation_controller(server_controller& controller)
            : _controller(controller) 
            {}
        /**
         * Gets rid of old authorisations
         * Also sends websocket the signal to stop the 
        */
        void authorisations_go_through(bool& end_requested);
        std::optional<authorisation> get_authorisation_by_id(const std::string& id);
        std::optional<authorisation> create_authorisation(const user& user, const app& app);
        /**
         * @return Either user ID or -1 if access_token is not supplied or -2 if access_token is supplied but not valid
        */
        int64_t is_legitimately_authorised(const crow::request&);
    };
}