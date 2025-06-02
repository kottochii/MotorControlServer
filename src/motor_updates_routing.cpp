#include <unordered_set>
#include "controllers/server_controller.hpp"
#include "controllers/http_controller.hpp"
#include "controllers/auth_controller.hpp"
#include "routing/motor_updates_routing.hpp"
#include "routing/commons.hpp"
#include "controllers/motor_updates_subscriptions_controller.hpp"

using namespace secsys_server;

extern server_controller* const app_server_controller;

void motor_updates_routing::route()
{
    auto http_controller = app_server_controller->get_http_controller();
    auto& app_ref = http_controller->get_app();
    CROW_WEBSOCKET_ROUTE(app_ref, "/motors_updates/subscribe")
        .onaccept(status_updates_onaccept)
        .onopen(status_updates_onopen)
        // .onmessage(status_updates_onmessage)
        // .onerror(status_updates_onerror)
        .onclose(status_updates_onclose);
}

// static std::unordered_map<
//         crow::websocket::connection*,
//         std::string
//     > connection_token;

bool motor_updates_routing::status_updates_onaccept(const crow::request& req, void** userdata)
{
    // needs to be authorised with a valid user
    auto auth = is_authorised(req);
    if(auth <= 0)
        return false;

    auto subs_ctl = app_server_controller->get_motor_updates_subscriptions_controller();
    *userdata = subs_ctl->create_user_data(req);

    return true;
}
void motor_updates_routing::status_updates_onopen(crow::websocket::connection& conn)
{    
    auto subs_ctl = app_server_controller->get_motor_updates_subscriptions_controller();
    // add connection to the list of active cnxns
    subs_ctl->add_connection(conn);
    return;
}
// void motor_updates_routing::status_updates_onmessage(crow::websocket::connection& conn, const std::string& message, bool is_binary)
// {
// 
// }
// void motor_updates_routing::status_updates_onerror(crow::websocket::connection& conn, const std::string& error_message)
// {

// }
void motor_updates_routing::status_updates_onclose(crow::websocket::connection& conn, const std::string& reason, uint16_t with_status_code)
{
    auto subs_ctl = app_server_controller->get_motor_updates_subscriptions_controller();
    // delete connection from the list
    subs_ctl->delete_connection(conn);
    // std::cout << "onclose:" << reason << ":" << with_status_code << std::endl;
}