#pragma once

#include "conncpp.hpp"
#include "Types.h"

namespace mariadb
{

    struct AuthSettings
    {
        String database;
        String login;
        String password;
    };
}