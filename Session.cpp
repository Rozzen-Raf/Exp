#include "Session.h"
//------------------------------------------------

Session::Session(Server* serv, Socket&& connection, ShedulerSharedPtr sheduler) noexcept : Connection(std::move(connection)), Sheduler(sheduler), Serv(serv)
{

}
//------------------------------------------------

Session::Session(Session&& right) : Connection(std::move(right.Connection)), Sheduler(std::exchange(right.Sheduler, nullptr)), Serv(std::exchange(right.Serv, nullptr))
{

}
//------------------------------------------------
Session::~Session()
{
    ///LOG(Session, "destroy");
}
//------------------------------------------------

CoroTaskVoid Session::AsyncRead(bool loop)
{
    while(loop)
    {
        buffer read_buffer_view(256);
        auto status = co_await Connection.async_read(Sheduler, read_buffer_view);

        if(status.type != WakeUp)
        {
            loop = false;
            Close();
        }
        Serv->RedirectAll(Connection.Desc(), read_buffer_view);
    }
    
    co_return;
}
//------------------------------------------------

CoroTaskVoid Session::AsyncWrite(buffer write_bf)
{
    auto status = co_await Connection.async_write(Sheduler, write_bf);

    if(status.type != WakeUp)
    {
        Close();
    }
    
    co_return;
}
//------------------------------------------------

void Session::Close() noexcept
{
    LOG(Session, "Close session");
    if(Serv)
        Serv->CloseSession(Connection.Desc());
}
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
