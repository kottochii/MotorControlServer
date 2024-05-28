#pragma once
#include <mosquittopp.h>
#include <string>
#include <optional>

namespace secsys_server
{
    class server_controller;
    struct arduino_mqtt_controller : public mosqpp::mosquittopp
    {
        arduino_mqtt_controller(server_controller& server, const std::string_view& topic, const char* host, int port, int keep_alive = 60);
        ~arduino_mqtt_controller() {
            disconnect();
        }

        arduino_mqtt_controller(const arduino_mqtt_controller& other) = default;
        arduino_mqtt_controller(arduino_mqtt_controller&& other) = default;

        void on_connect(int rc);
        void on_message(const struct mosquitto_message* message);
        void on_subscribe(int mid, int qos_client, const int* granted_qos);
        bool is_subscribed() const
        {
            return subscribed;
        }
        bool is_connected() const
        {
            return connected;
        }
        const std::string& get_topic()
        {
            return _topic;
        }
        private:
        bool subscribed = false;
        bool connected = false;
        server_controller& server;
        std::string _topic;
    };
}