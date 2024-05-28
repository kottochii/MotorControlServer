#include "controllers/http_controller.hpp"
#include "controllers/server_controller.hpp"
#include "crow.h"

namespace secsys_server
{
    http_controller::http_controller(server_controller& server_controller)
        : _server_controller(server_controller)
        , app()
    {
        app
            .multithreaded();
    }
}