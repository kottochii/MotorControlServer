#pragma once
#include <cstdint>
#include <string>
#include <crow.h>

namespace secsys_server
{
    struct app
    {
        friend class app_controller;

        uint32_t get_id() const
        {
            return _id;
        }
        const std::string& get_public_key() const
        {
            return _public_key;
        }
        std::optional<int> get_login_max_length() const
        {
            return _login_max_length;
        }

        void set_public_key(const std::string& public_key)
        {
            _public_key = public_key;
        }
        void set_login_max_length(std::optional<int> lgth)
        {
            _login_max_length = lgth;
        }

        operator crow::json::wvalue() const
        {
            return {
                {"id", get_id()},
                {"public_key", get_public_key()},
                {"login_max_length", get_login_max_length().has_value()?*get_login_max_length():((crow::json::wvalue){})}
            };
        }

        private:
        app(uint32_t id, const std::string& public_key, std::optional<int> login_max_length)
            : _id(id)
            , _public_key(public_key)
            , _login_max_length(login_max_length)
        {}
        app() = delete;
        uint32_t _id;
        std::string _public_key;
        std::optional<int> _login_max_length;
    };
}