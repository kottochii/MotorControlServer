#include "controllers/user_controller.hpp"
#include "controllers/server_controller.hpp"
#include "controllers/db_access_controller.hpp"
#include "helpers/strings.hpp"

using namespace secsys_server;

std::optional<user> user_controller::user_from_result(const pqxx::result& res)
{
    if(res.affected_rows() != 1)
        return std::nullopt;
    auto row = res[0];
    return (user){row["id"].as<uint32_t>(), row["pin"].as<std::string>(), row["name"].as<std::string>()};
}

user_controller::user_controller(server_controller& server_controller)
    : _controller(server_controller)
{}

std::optional<user> user_controller::get_by_id(uint32_t id)
{
    auto db_access = _controller.get_db_access_controller();
    auto cnxn = db_access->get_connection();
    pqxx::work txn(*cnxn);
    auto res = txn.exec("SELECT id, name, pin FROM users WHERE id=$1", pqxx::params{id});
    return user_from_result(res);
}

std::optional<user> user_controller::get_by_pin(const std::string& pin)
{
    auto db_access = _controller.get_db_access_controller();
    auto cnxn = db_access->get_connection();
    pqxx::work txn(*cnxn);
    auto res = txn.exec("SELECT id, name, pin FROM users WHERE pin=$1", pqxx::params{pin});
    return user_from_result(res);
}

std::optional<user> user_controller::get_by_card(const std::string& card_id, const std::string& data)
{
    auto db_access = _controller.get_db_access_controller();
    auto cnxn = db_access->get_connection();
    pqxx::work txn(*cnxn);
    auto res = txn.exec("SELECT u.id as \"id\", u.name as \"name\", u.pin as pin FROM users u JOIN cards ON cards.user_id=u.id AND cards.id=$1 AND cards.token=$2", pqxx::params{card_id, data});
    return user_from_result(res);
}

std::optional<std::string> user_controller::create_card(const std::string& card_id, uint32_t id)
{
    std::string token;
    {
        std::optional<user> auth;
        // the idea is to try 10 times to get random token that does not exist in the database
        for(int i = 0; i < 10; i++)
        {
            token = string_helpers::generate_random_string(32);
            auth = get_by_card(card_id, token);
            // we got the token that is not in the database, break out of the loop
            if(auth == std::nullopt)
                break;
        }
        // if couldn't get, then just fail the auth
        if(auth != std::nullopt)
            return std::nullopt;
    }
    auto db_access = _controller.get_db_access_controller();
    auto cnxn = db_access->get_connection();
    pqxx::work txn(*cnxn);
    auto res = txn.exec("INSERT INTO public.cards(id, token, user_id)	VALUES ($1, $2, $3);", pqxx::params{card_id, token, id});
    txn.commit();
    if(res.affected_rows() == 1)
        return token;
    return std::nullopt;
}