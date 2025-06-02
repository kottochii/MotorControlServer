#include "controllers/app_controller.hpp"
#include "controllers/motor_updates_subscriptions_controller.hpp"
#include "controllers/server_controller.hpp"
#include "controllers/motor_controller.hpp"

using namespace secsys_server;

void* motor_updates_subscriptions_controller:: create_user_data(const crow::request& req)
{
    user_data_t* __userdata = new user_data_t;
    __userdata->token = req.url_params.get("access_token");
    return (void*)__userdata;
}

void motor_updates_subscriptions_controller::add_connection(crow::websocket::connection& conn)
{
    connections.insert(&conn);
}

void motor_updates_subscriptions_controller::delete_connection(crow::websocket::connection& conn)
{
    auto* userdata = (motor_updates_subscriptions_controller::user_data_t*) conn.userdata();
    delete userdata;
    connections.erase(&conn);
}

void motor_updates_subscriptions_controller::status_updates_sendall(const std::string& what)
{
    for(auto& c : connections)
    {
        c->send_binary(what);
    }
}

void motor_updates_subscriptions_controller::status_updates_sendall()
{
    crow::json::wvalue::list _output = {};
    auto motor_controller = _controller.get_motor_controller();
    auto motors = motor_controller->get_all_motors();
    for(auto i = motors.begin(); i != motors.end(); i++)
    {
        _output.push_back((i->second));
    }


    status_updates_sendall(((crow::json::wvalue) {{"motors",_output}}).dump());
}

bool motor_updates_subscriptions_controller::unconnect_by_token(const std::string_view& token)
{
    for(auto& c : connections)
    {
        auto* userdata =  (motor_updates_subscriptions_controller::user_data_t*) c->userdata();
        if(userdata->token == token)
        {
            c->close("auth expired");
            return true;
        }
    }

    return false;
}