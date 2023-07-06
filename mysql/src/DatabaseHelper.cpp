#include "DatabaseHelper.hpp"
namespace mysql
{
    //-----------------------------------------------------------

    Database::Database(task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr mediator, const io::IPEndPoint& endpoint, const AuthSettings& auth) : 
    Sheduler(sheduler), 
    Mediator(mediator)
    {
        sheduler->CoroStart(Connect(endpoint, auth));
    }
    //-----------------------------------------------------------

    Database::~Database()
    {
        Close();
    }
    //-----------------------------------------------------------

    task::CoroTask<engine::AwaitableResult> Database::Connect(const io::IPEndPoint& endpoint, const AuthSettings& auth)
    {
        mysql_init(&MySQL);

        auto fd = MySQL.net.fd;

        net_async_status mysql_status;
        while((mysql_status = mysql_real_connect_nonblocking(&MySQL, endpoint.GetIP().c_str(), auth.login.c_str(), 
            auth.password.c_str(), auth.database.c_str(), endpoint.GetPort(), nullptr, 0)) == NET_ASYNC_NOT_READY)
        {
            auto status = co_await Sheduler->event(static_cast<engine::WorkerType>(Mediator->Type()), fd);
            if(!status)
                co_return status;

            continue;
        }

        if(mysql_status == NET_ASYNC_ERROR)
        {
            engine::AwaitableResult res;
            res.type = engine::Error;
            res.err_message = "mysql_real_connect_nonblocking() failed";
            ERROR(Database, res.err_message);
            co_return res;
        }

        co_return engine::AwaitableResult{engine::WakeUp};
    }
    //-----------------------------------------------------------

    void Database::Close()
    {
        mysql_close(&MySQL);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    //-----------------------------------------------------------
}