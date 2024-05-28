#include "routing/user_routing.hpp"
#include "controllers/server_controller.hpp"
#include "controllers/http_controller.hpp"
#include "controllers/user_controller.hpp"
#include "controllers/auth_controller.hpp"
#include "routing/commons.hpp"

#define CARD_CREATION_FAILED {{"id",700},{"description","Card creation failed"}}

using namespace secsys_server;
using namespace secsys_server::user_routing;

extern server_controller* const app_server_controller;


void user_routing::route()
{
    auto http_controller = app_server_controller->get_http_controller();
    auto& app_ref = http_controller->get_app();
    CROW_ROUTE(app_ref, "/users/id<int>")
        .methods(crow::HTTPMethod::GET)
        (get_by_id);
    CROW_ROUTE(app_ref, "/users/pin<string>")
        .methods(crow::HTTPMethod::GET)
        (get_by_pin);
    CROW_ROUTE(app_ref, "/users/register_card/<string>")
        .methods(crow::HTTPMethod::POST)
        (register_card);
}

crow::response user_routing::get_by_id(const crow::request& req, uint32_t id)
{
    auto usr = app_server_controller->get_user_controller()->get_by_id(id);

    if(usr.has_value())
    {
        return (crow::json::wvalue){{"response",*usr}};
    }
    return {404, (crow::json::wvalue){}};
}

crow::response user_routing::get_by_pin(const crow::request& req, const std::string& pin)
{
    auto usr = app_server_controller->get_user_controller()->get_by_pin(pin);

    if(usr.has_value())
    {
        return (crow::json::wvalue){{"response",*usr}};
    }
    return {404, (crow::json::wvalue){}};
}

crow::response user_routing::register_card(const crow::request& req, const std::string& card_id)
{
    check_authorisation(auth, req);
    auto usr_ctl = app_server_controller->get_user_controller();

    auto token = usr_ctl->create_card(card_id, auth);
    if(!token.has_value())
        return (crow::json::wvalue){
            {"error", CARD_CREATION_FAILED}
        };

    return (crow::json::wvalue){
        {"response", *token}
    };
}