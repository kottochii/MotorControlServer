#pragma once

#define is_authorised(request) app_server_controller->get_auth_controller()->is_legitimately_authorised(request)
// checks authorisation and returns 403 or 401, also packing the ID of user into `auth` variable
#define check_authorisation(auth, request) auto auth = is_authorised(request); \
    switch(auth) \
    { \
        case -1: return crow::response{403}; /* token is not supplied, Forbidden */ \
        case -2: return crow::response{401}; /* token is supplied but cannot be parsed, Unauthorized */ \
    }
