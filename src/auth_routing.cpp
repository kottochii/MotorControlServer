#include "controllers/server_controller.hpp"
#include "controllers/http_controller.hpp"
#include "controllers/auth_controller.hpp"
#include "controllers/user_controller.hpp"
#include "controllers/app_controller.hpp"
#include "routing/auth_routing.hpp"
#include "routing/commons.hpp"

using namespace secsys_server;
using namespace secsys_server::auth_routing;

extern server_controller* const app_server_controller;

void secsys_server::auth_routing::route()
{
    //app_server_controller = &server_controller;
    auto http_controller = app_server_controller->get_http_controller();
    auto& app_ref = http_controller->get_app();
    CROW_ROUTE(app_ref, "/auth/app<int>:<string>/card/<string>/<string>")
        .methods(crow::HTTPMethod::POST)
        (auth_with_card);
    CROW_ROUTE(app_ref, "/auth/app<int>:<string>/card/<string>/")
        .methods(crow::HTTPMethod::POST)
        (auth_with_card_id_only);
    CROW_ROUTE(app_ref, "/auth/app<int>:<string>/pin/<string>")
        .methods(crow::HTTPMethod::POST)
        (auth_with_pin);
}

crow::response auth_routing::auth_with_pin(const crow::request& req, int app_id, const std::string& app_public_key, const std::string& pin)
{
    auto app = app_server_controller->get_app_controller()->get_by_id(app_id);
    if(app == std::nullopt || (app->get_public_key() != app_public_key))
    {
        return {403, (crow::json::wvalue){{"error", "Application is not accessible"}}};
    }
    auto user = app_server_controller->get_user_controller()->get_by_pin(pin);
    // could not authorise
    if(user == std::nullopt)
    {
        return {401, (crow::json::wvalue){{"error", "PIN is not associated with any user"}}};
    }
    auto auth_obj = app_server_controller->get_auth_controller()->create_authorisation(*user, *app);
    if(!auth_obj.has_value())
        return {401, (crow::json::wvalue){{"error", "Authorization failed"}}};
    return {200, (crow::json::wvalue)
    {
        {"response",(crow::json::wvalue)*auth_obj}
    }
    };
}
crow::response auth_routing::auth_with_card(const crow::request& req, int app_id, const std::string& app_public_key, const std::string& card_id, const std::string& card_data)
{
    auto app = app_server_controller->get_app_controller()->get_by_id(app_id);
    if(app == std::nullopt || (app->get_public_key() != app_public_key))
    {
        return {403, (crow::json::wvalue){{"error", "Application is not accessible"}}};
    }
    auto user = app_server_controller->get_user_controller()->get_by_card(card_id, card_data);
    // could not authorise
    if(user == std::nullopt)
    {
        return {401, (crow::json::wvalue){{"error", "PIN is not associated with any user"}}};
    }
    auto auth_obj = app_server_controller->get_auth_controller()->create_authorisation(*user, *app);
    if(!auth_obj.has_value())
        return {401, (crow::json::wvalue){{"error", "Authorization failed"}}};
    return {200, (crow::json::wvalue)
    {
        {"response",(crow::json::wvalue)*auth_obj}
    }
    };
}

crow::response auth_routing::auth_with_card_id_only(const crow::request& req, int app_id, const std::string& app_public_key, const std::string& id)
{
    return auth_with_card(req, app_id, app_public_key, id, "");
}
