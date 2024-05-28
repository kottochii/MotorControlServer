#include "controllers/auth_controller.hpp"
#include "controllers/server_controller.hpp"
#include "helpers/strings.hpp"
#include <pqxx/pqxx>
#include <chrono>
#include "models/user_model.hpp"
#include "models/app_model.hpp"

using namespace secsys_server;

void authorisation_controller::authorisations_go_through(bool& end_requested)
{
    while(!end_requested)
    {
        pqxx::work w(*_controller.get_pqxx_connection());
        w.exec("DELETE FROM authorisations WHERE expires < NOW()");
        w.commit();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

std::optional<authorisation> authorisation_controller::get_authorisation_by_id(const std::string& id)
{
    auto cnxn = _controller.get_pqxx_connection();
    pqxx::work w(*cnxn);
    static bool prepped = false;
    if(!prepped)
    {
        prepped = true;
        cnxn->prepare("authorisation_controller_get_authorisation_by_id", "SELECT id, \"user\", app, extract(epoch from expires)::int as expires FROM authorisations WHERE id=$1");
    }
    auto res = w.exec_prepared("authorisation_controller_get_authorisation_by_id", id);
    return from_result(res);
}

std::optional<authorisation> authorisation_controller::from_result(const pqxx::result& res)
{
    if(res.affected_rows() != 1)
        return std::nullopt;
    auto row = res[0];

    return (authorisation){row["id"].as<std::string>(), row["user"].as<uint32_t>(), row["app"].as<uint32_t>(), row["expires"].as<uint32_t>()};
}

std::optional<authorisation> authorisation_controller::create_authorisation(const user& user, const app& app)
{
    std::string token;
    {
        std::optional<authorisation> auth;
        // the idea is to try 10 times to get random token that does not exist in the database
        for(int i = 0; i < 10; i++)
        {
            token = string_helpers::generate_random_string(64);
            auth = get_authorisation_by_id(token);
            // we got the token that is not in the database, break out of the loop
            if(auth == std::nullopt)
                break;
        }
        // if couldn't get, then just fail the auth
        if(auth != std::nullopt)
            return std::nullopt;
    }
    auto length = app.get_login_max_length();
    if(length.has_value())
        *length += time(NULL);
    authorisation _to_ret{token, user.get_id(), app.get_id(), length};
    // prepare everything for commit
    auto cnxn = _controller.get_pqxx_connection();
    pqxx::work w(*cnxn);
    static bool prepped = false;
    if(!prepped)
    {
        prepped = true;
        cnxn->prepare("authorisation_controller_create_authorisation", "INSERT INTO public.authorisations(id, \"user\", app, expires) VALUES ($1, $2, $3, (to_timestamp($4)));");
    }
    auto res = w.exec_prepared("authorisation_controller_create_authorisation", _to_ret.get_id(), _to_ret.get_user_id(), _to_ret.get_app_id(), _to_ret.get_expires());
    w.commit();
    if(res.affected_rows() == 1)
        return _to_ret;
    return std::nullopt;
}

int64_t authorisation_controller::is_legitimately_authorised(const crow::request& req)
{
    // first need the method, because need to know where to pull the request data from
    auto method = req.method;
    std::optional<std::string> token = std::nullopt;
    switch(method)
    {
    case crow::HTTPMethod::POST:
        {
            auto params = req.get_body_params();
            auto tok_ptr = params.get("access_token");
            if(tok_ptr != nullptr)
                token = tok_ptr;
        }
        break;
    case crow::HTTPMethod::GET:
        {
            auto tok_ptr = req.url_params.get("access_token");
            if(tok_ptr != nullptr)
                token = tok_ptr;
        }
        break;
    }
    // 403, no auth provided
    if(token == std::nullopt)
        return -1;
    else
    {
        auto auth = (get_authorisation_by_id(*token));
        if(auth.has_value())
        {
            return auth->get_user_id();
        }
        else return -2;
    }
}