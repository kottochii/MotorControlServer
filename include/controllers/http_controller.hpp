#pragma once
#include <iostream>
#include <crow.h>

namespace secsys_server
{
    class server_controller;
    struct http_controller
    {
        struct middleware_CORP
        {
            struct context 
            {};

            void before_handle(crow::request& req, crow::response& res, context& ctx){}

            void after_handle(crow::request& req, crow::response& res, context& ctx)
            {
                res.add_header("Cross-Origin-Resource-Policy", "cross-origin");
                res.add_header("Access-Control-Allow-Origin", "*");
            }
        };


        http_controller(server_controller& _server_controller);
        
        crow::App<middleware_CORP>& get_app()
        {
            return app;
        }

        private:
        server_controller& _server_controller;
        crow::App<middleware_CORP> app;
    };
}