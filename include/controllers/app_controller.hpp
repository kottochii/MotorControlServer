#pragma once
#include "models/app_model.hpp"
#include <optional>
#include <pqxx/pqxx>

namespace secsys_server
{
    class server_controller;
    class app_controller
    {
        server_controller& _controller;
        public:
        app_controller(server_controller& server_controller);
        std::optional<app> get_by_id(uint32_t id);

        static std::optional<app> app_from_result(const pqxx::result& res);
    };
}