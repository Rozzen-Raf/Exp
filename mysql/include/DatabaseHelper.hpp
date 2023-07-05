#pragma once

#include "MySQLTypes.hpp"
#include "IPEndPoint.h"
namespace mysql
{
    class Database
    {
    public:
        Database(const io::IPEndPoint& endpoint, const AuthSettings& auth);
        ~Database();

    private:
        bool Connect(const io::IPEndPoint& endpoint, const AuthSettings& auth);
        void Close();
    };
}
