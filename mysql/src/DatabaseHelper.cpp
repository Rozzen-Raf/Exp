#include "DatabaseHelper.hpp"
namespace mysql
{
    engine::AwaitableResult FailedLog(MYSQL* mysql, const String& method)
    {
        engine::AwaitableResult res;
        res.type = engine::Error;
        res.err = mysql_errno(mysql);
        res.err_message = mysql_error(mysql); 
        std::stringstream stream_err;
        stream_err << method << " failed. Error(" << res.err << "): " << res.err_message;
        ERROR(Database, stream_err.str());
        return res;
    }
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

        bool reg = false;

        net_async_status mysql_status;
        while((mysql_status = mysql_real_connect_nonblocking(&MySQL, endpoint.GetIP().c_str(), auth.login.c_str(), 
            auth.password.c_str(), auth.database.c_str(), endpoint.GetPort(), nullptr, 0)) == NET_ASYNC_NOT_READY)
        {
            auto fd = MySQL.net.fd;
            if(!reg)
                reg = Mediator->Register(fd) == 0;
            auto status = co_await Sheduler->event(static_cast<engine::WorkerType>(Mediator->Type()), fd);
            if(!status)
            {
                ERROR(Database, status.err_message);
                co_return status;
            }

            continue;
        }

        if(mysql_status == NET_ASYNC_ERROR)
        {
            engine::AwaitableResult res = FailedLog(&MySQL, "mysql_real_connect_nonblocking()");
            co_return res;
        }

        if(!reg)
            Mediator->Register(MySQL.net.fd);
        mIsConnection = true;

        co_return engine::AwaitableResult{engine::WakeUp};
    }
    //-----------------------------------------------------------

    task::CoroTask<QueryResult> Database::ExecuteQuery(const String& query)
    {
        QueryResult result;

        if(IsConnection() && query.size())
        {
            
        }

        co_return result;
    }
    //-----------------------------------------------------------

    void Database::Close()
    {
        mIsConnection = false;
        Mediator->Unregister(MySQL.net.fd);
        mysql_close(&MySQL);
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    //-----------------------------------------------------------

    Database::QueryHelper::QueryHelper(task::ShedulerSharedPtr sheduler, engine::WorkerType type) : Sheduler(sheduler), Type(type)
    {
    }
    //-----------------------------------------------------------

    task::CoroTask<engine::AwaitableResult> Database::QueryHelper::ExecuteQuery(MYSQL* mysql, const String& query)
    {
        engine::AwaitableResult status;
        net_async_status mysql_status;
        while((mysql_status = mysql_real_query_nonblocking(mysql, query.c_str(), query.size())) == NET_ASYNC_NOT_READY)
        {
            status = co_await Sheduler->event(static_cast<engine::WorkerType>(Type), mysql->net.fd);
            if(!status)
            {
                ERROR(Database, status.err_message);
                co_return status;
            }

            continue;
        }

        if(mysql_status == NET_ASYNC_ERROR)
        {
            status = FailedLog(mysql, "mysql_real_query_nonblocking()");
        }

        co_return status;
    }
    //-----------------------------------------------------------

    task::CoroTask<MYSQL_RES*> Database::QueryHelper::StoreResult(MYSQL* mysql)
    {
        MYSQL_RES* res = nullptr;

        engine::AwaitableResult status;
        net_async_status mysql_status;
        while((mysql_status = mysql_store_result_nonblocking(mysql, &res)) == NET_ASYNC_NOT_READY)
        {
            status = co_await Sheduler->event(static_cast<engine::WorkerType>(Type), mysql->net.fd);
            if(!status)
            {
                ERROR(Database, status.err_message);
                co_return nullptr;
            }

            continue;
        }

        if(mysql_status == NET_ASYNC_ERROR)
        {
            status = FailedLog(mysql, "mysql_store_result_nonblocking()");
            co_return nullptr;
        }

        co_return res;
    }
    //-----------------------------------------------------------

    task::CoroTask<MYSQL_ROW> Database::QueryHelper::FetchRow(MYSQL* mysql, MYSQL_RES* result)
    {
        MYSQL_ROW row;

        engine::AwaitableResult status;
        net_async_status mysql_status;
        while((mysql_status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_NOT_READY)
        {
            status = co_await Sheduler->event(static_cast<engine::WorkerType>(Type), mysql->net.fd);
            if(!status)
            {
                ERROR(Database, status.err_message);
                co_return nullptr;
            }

            continue;
        }

        if(mysql_status == NET_ASYNC_ERROR)
        {
            status = FailedLog(mysql, "mysql_fetch_row_nonblocking()");
            co_return nullptr;
        }

        co_return row;
    }
    //-----------------------------------------------------------

    task::CoroTaskVoid Database::QueryHelper::FreeResult(MYSQL* mysql, MYSQL_RES* result)
    {
        engine::AwaitableResult status;
        net_async_status mysql_status;
        while((mysql_status = mysql_free_result_nonblocking(result)) == NET_ASYNC_NOT_READY)
        {
            status = co_await Sheduler->event(static_cast<engine::WorkerType>(Type), mysql->net.fd);
            if(!status)
            {
                ERROR(Database, status.err_message);
                co_return;
            }

            continue;
        }

        if(mysql_status == NET_ASYNC_ERROR)
        {
            status = FailedLog(mysql, "mysql_free_result_nonblocking()");
            co_return;
        }

        co_return;
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    //-----------------------------------------------------------