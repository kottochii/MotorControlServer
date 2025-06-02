#pragma once
#include <unordered_map>
#include <unordered_set>
#include <array>
#include "models/motor_model.hpp"
#include "config/server_config.hpp"
#include <chrono>

namespace secsys_server
{
    class server_controller;

    class motor_updates_subscriptions_controller
    {
        server_controller& _controller;

        std::unordered_set<crow::websocket::connection*> connections;


        public:
        typedef struct 
        {
            std::string token;
        } user_data_t;

        /**
         * Creates a pointer to token that can be used in 
         * @returns
         * pointer to the first character of where the token was inserted 
         * (ie. something that can be used later to find out whether the token is in the queue)
         */
        void* create_user_data(const crow::request& req);

        /**
         * Adds connection to the controller
         */
        void add_connection(crow::websocket::connection& conn);

        /**
         * Adds connection to the controller
         */
        void delete_connection(crow::websocket::connection& conn);


        /**
         * Sends `what` to all connected users
         */
        void status_updates_sendall(const std::string& what);

        /**
         * Sends data about current state of motors to all connected users
         */
        void status_updates_sendall();

        /**
         * close connection with the client by token
         */
        bool unconnect_by_token(const std::string_view& token);


        motor_updates_subscriptions_controller(server_controller& controller) 
                                                                            : _controller(controller)
                                                                            {};
        motor_updates_subscriptions_controller() = delete;

    };
}