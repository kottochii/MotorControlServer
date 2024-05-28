#pragma once
#include <cstdint>
#include <string>
#include <chrono>
#include <crow.h>

namespace secsys_server
{
    struct motor
    {
        enum motor_state
        {
            OPENING, CLOSING, STOPPED
        };
        // an observer of motors, keeps track of all motors
        friend class motor_controller;
        std::string get_id() const
        {
            return _id;
        }

        bool online() const
        {
            using namespace std::chrono;
            return duration_cast<seconds>(system_clock::now() - _last_online).count() < 5;
        }
        double get_opened_degree() const
        {
            return _opened_degree;
        }
        motor_state get_state() const
        {
            return _state;
        }
        std::string get_state_str() const
        {
            switch(_state)
            {
                case OPENING:
                    return "opening";
                case CLOSING:
                    return "closing";
                case STOPPED:
                default:
                    return "stopped";
            }
        }

        operator crow::json::wvalue ()
        {
            crow::json::wvalue returnable {
                {"id", _id},
                {"online", online()}
            };
            if(online())
            {
                using namespace std::chrono;
                returnable["state"] = get_state_str();
                returnable["open_degree"] = get_opened_degree();
            }
            return returnable;
        }

        private:
        motor(const std::string_view& id)
            : _id(id)
        { }
        std::string _id;
        double _opened_degree = 0;
        motor_state _state = STOPPED;
        std::chrono::time_point<std::chrono::system_clock> _last_online = std::chrono::system_clock::now();
    };
}