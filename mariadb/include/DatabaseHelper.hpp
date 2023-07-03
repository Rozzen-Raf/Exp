#pragma once

#include "MariadDBTypes.hpp"
#include "IPEndPoint.h"
namespace mariadb
{
    class Database
    {
    public:
        Database(const io::IPEndPoint& endpoint, const AuthSettings& auth);
        ~Database();

    public:
        ResultSetSharedPtr ExecuteQuery(const String& query);

    private:
        bool Connect(const io::IPEndPoint& endpoint, const AuthSettings& auth);
        void Close();
        ConnectionUniquePtr ConnPtr;
    };
}
