#pragma once
#include <string>
#include <optional>
#include <memory>
#include <pqxx/pqxx>


namespace secsys_server
{
    struct arduino_mqtt_controller;
    class http_controller;
    class user_controller;
    class motor_controller;
    class authorisation_controller;
    class app_controller;
    class server_controller
    {
        std::shared_ptr<arduino_mqtt_controller> _mqtt_controller_ref;
        std::shared_ptr<http_controller> _http_controller_ref;
        std::shared_ptr<pqxx::connection> _pqxx_connection_ref;
        std::shared_ptr<user_controller> _user_controller;
        std::shared_ptr<motor_controller> _motor_controller;
        std::shared_ptr<authorisation_controller> _auth_controller;
        std::shared_ptr<app_controller> _app_controller;
        public:
        /**
         * Should be called by mosquitto message handler
         * @param msg Message
         * @param topic Topic at which the message was caught
        */
        void on_mqtt_message_received(const std::string_view& msg, const std::string& topic);
        void set_mqtt_controller(std::shared_ptr<arduino_mqtt_controller> mqtt_init);
        void set_http_controller(std::shared_ptr<http_controller> http_init);
        void set_pqxx_connection(std::shared_ptr<pqxx::connection> ref);
        void set_user_controller(std::shared_ptr<user_controller> ref);
        void set_motor_controller(std::shared_ptr<motor_controller> ref);
        void set_auth_controller(std::shared_ptr<authorisation_controller> ref);
        void set_app_controller(std::shared_ptr<app_controller> ref);

        std::shared_ptr<arduino_mqtt_controller> get_mqtt_controller();
        std::shared_ptr<http_controller> get_http_controller();
        std::shared_ptr<pqxx::connection> get_pqxx_connection();
        std::shared_ptr<user_controller> get_user_controller();
        std::shared_ptr<motor_controller> get_motor_controller();
        std::shared_ptr<authorisation_controller> get_auth_controller();
        std::shared_ptr<app_controller> get_app_controller();
    };
}