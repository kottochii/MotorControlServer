#include "controllers/app_controller.hpp"
#include "controllers/server_controller.hpp"

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
    auto cnxn = _controller.get_pqxx_connection();
    pqxx::work txn(*cnxn);
    static bool prepped = false;
    if(!prepped)
    {
        prepped = true;
        cnxn->prepare("app_controller_get_by_id_prep", "SELECT id, public_key, login_max_length FROM public.apps WHERE id=$1");
    }
    auto res = txn.exec_prepared("app_controller_get_by_id_prep", id);
    return app_from_result(res);
}