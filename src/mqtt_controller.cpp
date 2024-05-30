#include "controllers/mqtt_controller.hpp"
#include "controllers/server_controller.hpp"
#include "except.hpp"
#include <iostream>
#include <thread>

using namespace secsys_server;

arduino_mqtt_controller::arduino_mqtt_controller(server_controller& server, const std::string_view& topic, const char* host, int port, int keep_alive)
    : server(server)
    , _topic(topic)
{
    connect(host, port, keep_alive);
}

void arduino_mqtt_controller::on_connect(int rc)
{
	if(rc == 0){
        connected = true;
		/* Only attempt to subscribe on a successful connect. */
		subscribe(NULL, _topic.c_str());
	}
    else throw mqtt_connection_failed("MQTT connection failed, server might be offline.");
}

void arduino_mqtt_controller::on_message(const struct mosquitto_message* message)
{
    std::string_view msg {(char*)message->payload, (unsigned)message->payloadlen};
    server.on_mqtt_message_received(msg, message->topic);
}

void arduino_mqtt_controller::on_subscribe(int mid, int qos_client, const int* granted_qos)
{
    subscribed = true;
}

void arduino_mqtt_controller::report_online(bool& end_requested)
{
    using namespace std::chrono_literals;
    while(!end_requested)
    {
        std::string msg {"server ok"};
        publish(NULL, get_topic().c_str(), msg.length(), msg.c_str());
        std::this_thread::sleep_for(5s);
    }
}