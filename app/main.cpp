#ifdef SSL_REQUIRED
#define CROW_ENABLE_SSL
#endif
#include <iostream>
#include "signal_handler.hpp"
#include "controllers/mqtt_controller.hpp"
#include "controllers/server_controller.hpp"
#include "controllers/http_controller.hpp"
#include "controllers/user_controller.hpp"
#include "controllers/motor_controller.hpp"
#include "controllers/auth_controller.hpp"
#include "controllers/app_controller.hpp"
#include "routing/motor_routing.hpp"
#include "routing/user_routing.hpp"
#include "routing/auth_routing.hpp"
#include "config/server_config.hpp"
#include <pqxx/pqxx>

#define CONFIG_FILE     "/etc/motor_server/config.json"

bool end_requested = false;
// extern declaration for the handler
extern struct signal_handler_t signal_handler;
// the handler itself
void signal_handler_t::on_sig_int(int s)
{
    // will still continue
    std::cout << "Exiting..." << std::endl;
    end_requested = true;
}

using namespace secsys_server;
// create a server controller
server_controller srv_controller;
extern server_controller* const app_server_controller;
server_controller* const app_server_controller = &srv_controller;

// create configuration file
auto config = server_config::from_file(CONFIG_FILE);
int main()
{
    if(!config.has_value())
    {
        exit(0);
    }


    // create an MQTT connection/controller
    auto mqtt_controller = std::make_shared<arduino_mqtt_controller>(srv_controller, config->mqtt_topic, config->mqtt_host.c_str(), config->mqtt_port);
    // tie it to the server controller
    srv_controller.set_mqtt_controller(mqtt_controller);
    // create a HTTP controller
    auto http_controller_v = std::make_shared<http_controller>(srv_controller);
    // tie it to the server controller
    srv_controller.set_http_controller(http_controller_v);
    // create a PQXX connection and associate it with the server
    auto pqxx_connection = std::make_shared<pqxx::connection>(config->pqxx_connection_string);
    srv_controller.set_pqxx_connection(pqxx_connection);
    // create a user controller
    auto user_controller_v = std::make_shared<user_controller>(srv_controller);
    // tie to the server
    srv_controller.set_user_controller(user_controller_v);
    // create a motor controller
    auto motor_controller_v = std::make_shared<motor_controller>(srv_controller, config->motors);
    // tie to the server
    srv_controller.set_motor_controller(motor_controller_v);

    // create a motor controller
    auto auth_controller_v = std::make_shared<authorisation_controller>(srv_controller);
    // tie to the server
    srv_controller.set_auth_controller(auth_controller_v);
    // create a motor controller
    auto app_controller_v = std::make_shared<app_controller>(srv_controller);
    // tie to the server
    srv_controller.set_app_controller(app_controller_v);

    // set up the router
    motor_routing::route();
    user_routing::route();
    auth_routing::route();


    // run HTTP and MQTT
    mqtt_controller->loop_start();
    auto& app = http_controller_v->get_app();
    #ifdef SSL_REQUIRED
    if(config->ssl_certificate.has_value() && config->ssl_keyfile.has_value())
    {
        //asio::ssl::context ctx(asio::ssl::context::sslv23);
        app.ssl_file(*config->ssl_certificate, *config->ssl_keyfile);
        //app.ssl(ctx);
    }
    else 
    {
        std::cerr << "SSL certificate file or keyfile is not found." << std::endl << 
        "Try either specifying them in config.json or recompiling the program without -DSSL_REQUIRED." << std::endl;
        exit(0);
    }
    #endif
    auto future = app.port(config->http_port).signal_clear().run_async();

    // motor controller async
    auto observer_future = std::async(std::launch::async, [&motor_controller_v](){
        using namespace std::chrono_literals;
        while(!end_requested)
        {
            motor_controller_v->motors_go_through();
            std::this_thread::sleep_for(1s);
        }
    });

    auto auth_gt_ftr = std::async(std::launch::async, &authorisation_controller::authorisations_go_through, auth_controller_v, std::ref(end_requested));

    auto server_reporting_ftr = std::async(std::launch::async, &arduino_mqtt_controller::report_online, mqtt_controller, std::ref(end_requested));

    // wait for interruption
    while(!end_requested)
    {
        sleep(1);
    }
    app.stop();
    return 0;
}