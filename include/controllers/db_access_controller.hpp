#pragma once
#include <iostream>
#include <memory>
#include <pqxx/pqxx>

namespace secsys_server
{
    class server_controller;
    class db_access_controller
    {
        std::string _connection_string;

        public:
        db_access_controller(const std::string_view& connection_string)
                                                    : _connection_string(connection_string)
                                                    {}
        std::unique_ptr<pqxx::connection> get_connection() const;
    };
}