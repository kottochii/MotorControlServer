#pragma once
#include <unordered_map>
#include <array>
#include "models/motor_model.hpp"
#include "config/server_config.hpp"

namespace secsys_server
{
    class server_controller;

    class motor_controller
    {
        server_controller& _controller;
        std::unordered_map<std::string, motor> _motor_observer;

        public:
        motor_controller(server_controller& controller, std::unordered_map<std::string, server_config::motor_config> motors)
            : _controller(controller)
            , _motor_observer()
        {
            for(auto i = motors.begin(); i != motors.end(); i++)
            {
                _motor_observer.insert({i->first, {i->first}});
            }
        }
        std::optional<motor> get_motor_by_id(const std::string& id) const;
        const std::unordered_map<std::string, motor> get_all_motors()
        {
            return _motor_observer;
        }

        void motors_go_through();

        void update_motor_status(const std::string& id, double open_degree, motor::motor_state state);

        void open(const motor& mtr);

        void close(const motor& mtr);

        void stop(const motor& mtr);
    };
}