#pragma once

#include "MariadDBTypes.hpp"
#include "IPEndPoint.h"
namespace mariadb
{
    using Connection = sql::Connection;
    UNIQUE_PTR(Connection);
    class Database
    {
    public:
        Database(const io::IPEndPoint& endpoint, const AuthSettings& auth);
        ~Database() = default;

    private:
        bool Connect(const io::IPEndPoint& endpoint, const AuthSettings& auth);
        ConnectionUniquePtr ConnPtr;
    };
}
