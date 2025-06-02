#include "controllers/db_access_controller.hpp"

using namespace secsys_server;

std::unique_ptr<pqxx::connection> db_access_controller::get_connection() const
{
    auto cnxn_str = _connection_string;
    auto u_ptr = std::make_unique<pqxx::connection>(_connection_string);
    return u_ptr;
}