#include "routing/motor_routing.hpp"
#include "controllers/server_controller.hpp"
#include "controllers/http_controller.hpp"
#include "controllers/motor_controller.hpp"
#include "controllers/auth_controller.hpp"
#include "routing/commons.hpp"

#define DEVICE_OFFLINE {{"id",800},{"description","Device is offline"}}

using namespace secsys_server;
using namespace secsys_server::motor_routing;

extern server_controller* const app_server_controller;


void motor_routing::route()
{
    auto http_controller = app_server_controller->get_http_controller();
    auto& app_ref = http_controller->get_app();
    CROW_ROUTE(app_ref, "/motors/")
        .methods(crow::HTTPMethod::GET)
        (get_all);
    CROW_ROUTE(app_ref, "/motors/<string>")
        .methods(crow::HTTPMethod::GET)
        (get_by_id);
    CROW_ROUTE(app_ref, "/motors/<string>/open")
        .methods(crow::HTTPMethod::POST)
        (open_by_id);
    CROW_ROUTE(app_ref, "/motors/<string>/stop")
        .methods(crow::HTTPMethod::POST)
        (stop_by_id);
    CROW_ROUTE(app_ref, "/motors/<string>/close")
        .methods(crow::HTTPMethod::POST)
        (close_by_id);
}

crow::response motor_routing::get_all(const crow::request& req)
{
    check_authorisation(auth, req);
    crow::json::wvalue::list _output = {};
    auto motors = app_server_controller->get_motor_controller()->get_all_motors();
    for(auto i = motors.begin(); i != motors.end(); i++)
    {
        _output.push_back((i->second));
    }
    return (crow::json::wvalue){{"response",_output}};
}

crow::response motor_routing::get_by_id(const crow::request& req, const std::string& id)
{
    check_authorisation(auth, req);
    auto motor = app_server_controller->get_motor_controller()->get_motor_by_id(id);
    if(motor.has_value())
    {
        return (crow::json::wvalue){{"response",*motor}};
    }
    return crow::response(404);
}

crow::response motor_routing::open_by_id(const crow::request& req, const std::string& id)
{
    check_authorisation(auth, req);
    auto motor_controller = app_server_controller->get_motor_controller();
    auto motor = motor_controller->get_motor_by_id(id);
    if(motor.has_value())
    {
        if(motor->online())
        {
            motor_controller->open(*motor);
            return crow::response(204);
        }
        else
        {
            return crow::response(503, (crow::json::wvalue){{"error",DEVICE_OFFLINE}});
        }
    }
    return crow::response(404);
}

crow::response motor_routing::close_by_id(const crow::request& req, const std::string& id)
{
    check_authorisation(auth, req);
    auto motor_controller = app_server_controller->get_motor_controller();
    auto motor = motor_controller->get_motor_by_id(id);
    if(motor.has_value())
    {
        if(motor->online())
        {
            motor_controller->close(*motor);
            return crow::response(204);
        }
        else return crow::response(503, (crow::json::wvalue){{"error",DEVICE_OFFLINE}});
    }
    return crow::response(404);
}

crow::response motor_routing::stop_by_id(const crow::request& req, const std::string& id)
{
    check_authorisation(auth, req);
    auto motor_controller = app_server_controller->get_motor_controller();
    auto motor = motor_controller->get_motor_by_id(id);
    if(motor.has_value())
    {
        if(motor->online())
        {
            motor_controller->stop(*motor);
            return crow::response(204);
        }
        else return crow::response(503, (crow::json::wvalue){{"error",DEVICE_OFFLINE}});
    }
    return crow::response(404);
}
