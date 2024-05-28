#pragma once
#include <iostream>
#include <crow.h>

namespace secsys_server
{
    class server_controller;
    struct http_controller
    {
        http_controller(server_controller& _server_controller);
        
        crow::SimpleApp& get_app()
        {
            return app;
        }

        private:
        server_controller& _server_controller;
        crow::SimpleApp app;
    };
}