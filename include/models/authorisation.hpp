#pragma once
#include <crow/json.h>

namespace secsys_server
{
    struct user;
    struct app;
    struct authorisation
    {
        friend class authorisation_controller;
        const std::string& get_id() const
        {
            return _id;
        }
        uint32_t get_user_id() const
        {
            return _user_id;
        }
        uint32_t get_app_id() const
        {
            return _app_id;
        }
        auto get_expires() const 
        {
            return _expires;
        }
        authorisation(const std::string& id, uint32_t user_id, uint32_t app_id, std::optional<uint32_t> expires)
            : _id(id)
            , _user_id(user_id)
            , _app_id(app_id)
            , _expires(expires)
        {}
        operator crow::json::wvalue()
        {
            crow::json::wvalue wv{}; 
            wv["token"] = _id;
            wv["expires"] = _expires.has_value()?(*_expires):(crow::json::wvalue{});
            return wv;
        }
        private:
        std::string _id;
        uint32_t _user_id;
        uint32_t _app_id;
        std::optional<uint32_t> _expires;
    };
}