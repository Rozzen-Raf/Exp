#pragma once

#include "MySQLTypes.hpp"
#include "IPEndPoint.h"
#include "Sheduler.h"
#include "RegisterMediator.h"
namespace mysql
{
    class Database
    {
    public:
        Database(task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr mediator, const io::IPEndPoint& endpoint, const AuthSettings& auth);
        ~Database();

    private:
        task::CoroTask<engine::AwaitableResult> Connect(const io::IPEndPoint& endpoint, const AuthSettings& auth);
        void Close();

        MYSQL MySQL;
        task::ShedulerSharedPtr Sheduler;
        engine::RegisterMediatorBasePtr Mediator;
    };
}
