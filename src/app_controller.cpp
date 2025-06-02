#include "controllers/app_controller.hpp"
#include "controllers/server_controller.hpp"
#include "controllers/db_access_controller.hpp"

using namespace secsys_server;

std::optional<app> app_controller::app_from_result(const pqxx::result& res)
{
    if(res.affected_rows() != 1)
        return std::nullopt;
    auto row = res[0];
    return (app){row["id"].as<uint32_t>(), row["public_key"].as<std::string>(), row["login_max_length"].as<std::optional<int>>()};
}

app_controller::app_controller(server_controller& controller)
: _controller(controller)
{

}

std::optional<app> app_controller::get_by_id(uint32_t id)
{
    auto cnxn = _controller.get_db_access_controller()->get_connection();
    pqxx::work txn(*cnxn);
    auto res = txn.exec("SELECT id, public_key, login_max_length FROM public.apps WHERE id=$1", pqxx::params {id});
    return app_from_result(res);
}