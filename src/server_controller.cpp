#include "controllers/server_controller.hpp"
#include "controllers/mqtt_controller.hpp"
#include "controllers/http_controller.hpp"
#include "controllers/motor_controller.hpp"
#include "controllers/app_controller.hpp"
#include "helpers/strings.hpp"

using namespace secsys_server;

void server_controller::on_mqtt_message_received(const std::string_view& msg, const std::string& topic)
{
    //std::cout << msg << "::" << topic << std::endl;
    if(msg.starts_with("state "))
    {
        std::cout << "received status" << std::endl;
        auto vc = string_helpers::tokenize(msg, (std::string_view)" ");
        for(auto& i : vc)
        {
            std::cout << i << std::endl;
        }
        if(vc.size() != 4 && vc.size() != 5)
            return;
        std::string id = {vc[1].begin(), vc[1].end()};
        double open_degree;
        {
            auto fc_r = std::from_chars(vc[2].begin(), vc[2].begin() + vc[2].size(), open_degree);
            if(fc_r.ec != std::errc{})
            {
                return;
            }
        }
        motor::motor_state state;
        {
            if(vc[3] == "moving")
            {
                if(vc.size() >= 5)
                {
                    if(vc[4] == "open")
                        state = motor::OPENING;
                    else if(vc[4] == "close")
                        state = motor::CLOSING;
                    else return;
                }
            }
            else if(vc[3] == "stopped")
                state = motor::STOPPED;
            else return;
        }
        auto mc = get_motor_controller();
        mc->update_motor_status(id, open_degree, state);
    }
}


void server_controller::set_mqtt_controller(std::shared_ptr<arduino_mqtt_controller> mqtt_init)
{
    _mqtt_controller_ref = mqtt_init;
}
void server_controller::set_http_controller(std::shared_ptr<http_controller> http_init)
{
    _http_controller_ref = http_init;
}
void server_controller::set_pqxx_connection(std::shared_ptr<pqxx::connection> ref)
{
    _pqxx_connection_ref = ref;
}
void server_controller::set_user_controller(std::shared_ptr<user_controller> ref)
{
    _user_controller = ref;
}
void server_controller::set_motor_controller(std::shared_ptr<motor_controller> ref)
{
    _motor_controller = ref;
}
void server_controller::set_auth_controller(std::shared_ptr<authorisation_controller> ref)
{
    _auth_controller = ref;
}
void server_controller::set_app_controller(std::shared_ptr<app_controller> ref)
{
    _app_controller = ref;
}


std::shared_ptr<arduino_mqtt_controller> server_controller::get_mqtt_controller()
{
    return _mqtt_controller_ref;
}
std::shared_ptr<http_controller> server_controller::get_http_controller()
{
    return _http_controller_ref;
}
std::shared_ptr<pqxx::connection> server_controller::get_pqxx_connection()
{
    return _pqxx_connection_ref;
}
std::shared_ptr<user_controller> server_controller::get_user_controller()
{
    return _user_controller;
}
std::shared_ptr<motor_controller> server_controller::get_motor_controller()
{
    return _motor_controller;
}
std::shared_ptr<authorisation_controller> server_controller::get_auth_controller()
{
    return _auth_controller;
}
std::shared_ptr<app_controller> server_controller::get_app_controller()
{
    return _app_controller;
}