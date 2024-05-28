#include "controllers/motor_controller.hpp"
#include "controllers/server_controller.hpp"
#include "controllers/mqtt_controller.hpp"

namespace secsys_server
{
    std::optional<motor> motor_controller::get_motor_by_id(const std::string& id) const
    {
        auto it = _motor_observer.find(id);
        if( it != _motor_observer.end() )
            return it->second;
        return std::nullopt;
    }

    void motor_controller::motors_go_through()
    {
        for(auto i = _motor_observer.begin(); i != _motor_observer.end(); i++)
        {
            if(!i->second.online())
            {
                using namespace std::string_literals;
                auto mqtt = _controller.get_mqtt_controller();
                std::string msg {"status "s + (i->first)};
                mqtt->publish(NULL, mqtt->get_topic().c_str(), msg.length(), msg.c_str());
            }
        }
    }

    void motor_controller::update_motor_status(const std::string& id, double open_degree, motor::motor_state state)
    {
        auto it = _motor_observer.find(id);
        if( it != _motor_observer.end() )
        {
            it->second._state = state;
            it->second._opened_degree = open_degree;
            it->second._last_online = std::chrono::system_clock::now();
        }
    }

    void motor_controller::open(const motor& mtr)
    {
        if(mtr.online())
        {
            using namespace std::string_literals;
            auto mqtt = _controller.get_mqtt_controller();
            std::string msg {"open "s + (mtr.get_id())};
            mqtt->publish(NULL, mqtt->get_topic().c_str(), msg.length(), msg.c_str());
        }
    }

    void motor_controller::close(const motor& mtr)
    {
        if(mtr.online())
        {
            using namespace std::string_literals;
            auto mqtt = _controller.get_mqtt_controller();
            std::string msg {"close "s + (mtr.get_id())};
            mqtt->publish(NULL, mqtt->get_topic().c_str(), msg.length(), msg.c_str());
        }
    }

    void motor_controller::stop(const motor& mtr)
    {
        if(mtr.online())
        {
            using namespace std::string_literals;
            auto mqtt = _controller.get_mqtt_controller();
            std::string msg {"stop "s + (mtr.get_id())};
            mqtt->publish(NULL, mqtt->get_topic().c_str(), msg.length(), msg.c_str());
        }
    }

}