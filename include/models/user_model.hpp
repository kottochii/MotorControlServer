#pragma once
#include <cstdint>
#include <string>
#include <bitset>
#include <crow.h>


namespace secsys_server
{
    struct user
    {
        friend class user_controller;
        uint32_t get_id() const
        {
            return _id;
        }
        std::string get_pin() const
        {
            return _pin;
        }
        std::string get_name() const
        {
            return _name;
        }

        void set_pin(const std::string_view& v)
        {
            _pin = v;
            _changes[PIN] = 1;
        }
        void set_name (const std::string_view& v)
        {
            _name = v;
            _changes[NAME] = 1;
        }

        operator crow::json::wvalue() const
        {
            return {
                {"id", get_id()},
                {"pin", get_pin()},
                {"name", get_name()}
            };
        }

        private:
        user(uint32_t id, const std::string& pin, const std::string& name)
            : _id(id)
            , _pin(pin)
            , _name(name)
        { }
        user() = delete;
        uint32_t _id;
        std::string _pin;
        std::string _name;
        std::bitset<2> _changes = 0;
        enum changes_bits
        {
            PIN, NAME
        };
    };
}   