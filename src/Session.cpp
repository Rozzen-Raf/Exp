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
        buffer_ptr read_buffer = std::make_shared<buffer>();
        auto status = co_await Connection.async_read(Sheduler, read_buffer);

        if(status.type != WakeUp)
        {
            loop = false;
            ERROR(Session, status.err_message);
            Close();
        }
        Serv->RedirectAll(Connection.Desc(), read_buffer);
    }
    
    co_return;
}
//------------------------------------------------

CoroTaskVoid Session::AsyncWrite(buffer_ptr write_bf)
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
