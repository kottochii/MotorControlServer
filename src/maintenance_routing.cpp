#include "controllers/server_controller.hpp"
#include "controllers/http_controller.hpp"
#include "controllers/app_controller.hpp"
#include "routing/maintenance_routing.hpp"

#define BAD_APP_DATA_RESPONSE       {401, (crow::json::wvalue){{"error", "Application does not exist"}}}

using namespace secsys_server;
using namespace secsys_server::maintenance_routing;

extern server_controller* const app_server_controller;

void secsys_server::maintenance_routing::route()
{
    //app_server_controller = &server_controller;
    auto http_controller = app_server_controller->get_http_controller();
    auto& app_ref = http_controller->get_app();
    CROW_ROUTE(app_ref, "/maintenance/app<int>:<string>/hello")
        .methods(crow::HTTPMethod::GET)
        (hello);
}

crow::response maintenance_routing::hello(const crow::request& req, int app_id, const std::string& app_public_key)
{
    auto app = app_server_controller->get_app_controller()->get_by_id(app_id);
    if(app == std::nullopt)
    {
        return BAD_APP_DATA_RESPONSE;
    }
    else if(app->get_public_key() != app_public_key)
    {
        return BAD_APP_DATA_RESPONSE;
        // return {403, (crow::json::wvalue){{"error", "Application authorisation failed"}}};
    }
    return (crow::response)(204);
}
