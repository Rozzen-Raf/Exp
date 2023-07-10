#pragma once

#include "MySQLTypes.hpp"
#include "IPEndPoint.h"
#include "Sheduler.h"
#include "RegisterMediator.h"
#include "Socket.h"
#include "QueryResult.hpp"
namespace mysql
{
    engine::AwaitableResult FailedLog(MYSQL* mysql, const String& method);

    class Database
    {
    public:
        Database(task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr mediator, const io::IPEndPoint& endpoint, const AuthSettings& auth);
        ~Database();

        inline bool IsConnection() const noexcept { return mIsConnection; }

        task::CoroTask<QueryResult> ExecuteQuery(const String& query);
    private:
        class QueryHelper
        {   
        public:
            QueryHelper(task::ShedulerSharedPtr sheduler, engine::WorkerType type);
            task::CoroTask<engine::AwaitableResult> ExecuteQuery(MYSQL* mysql, const String& query);
            task::CoroTask<MYSQL_RES*> StoreResult(MYSQL* mysql);
            task::CoroTask<MYSQL_ROW> FetchRow(MYSQL* mysql, MYSQL_RES* result);
            task::CoroTaskVoid FreeResult(MYSQL* mysql, MYSQL_RES* result);

        private:
            task::ShedulerSharedPtr Sheduler;
            engine::WorkerType Type;
        };

        task::CoroTask<engine::AwaitableResult> Connect(const io::IPEndPoint& endpoint, const AuthSettings& auth);
        void Close();

        MYSQL MySQL;
        task::ShedulerSharedPtr Sheduler;
        engine::RegisterMediatorBasePtr Mediator;
        io::Socket mSocket;
        bool mIsConnection = false;
    };
}
