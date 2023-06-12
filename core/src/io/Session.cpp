#include "Session.h"
namespace io
{
//------------------------------------------------

Session::Session(Server* serv, Socket&& connection, task::ShedulerSharedPtr sheduler) noexcept : Connection(std::move(connection)), Sheduler(sheduler), Serv(serv)
{

}
//------------------------------------------------

Session::Session(Session&& right) : Connection(std::move(right.Connection)), Sheduler(std::exchange(right.Sheduler, nullptr)), Serv(std::exchange(right.Serv, nullptr))
{

}
//------------------------------------------------
Session::~Session()
{
    IsWorking = false;
}
//------------------------------------------------

task::CoroTaskVoid Session::AsyncRead(bool loop, engine::WorkerType type, uint size_buffer, handler_packet_f handler_packet)
{
    ASSERT(handler_packet);

    IsWorking = true;
    
    do{
        String buf(size_buffer, size_buffer);
        buffer_view read_buffer = std::as_writable_bytes(std::span(buf));

        auto status = co_await Connection.async_read(Sheduler.get(), read_buffer, type);

        if(!IsWorking)
            co_return;

        if(!status)
        {
            loop = false;
            ERROR(Session, status.err_message);
            Close();
            co_return;
        }

        String res;
        res = co_await handler_packet(read_buffer);

        SendResult(res);
    }while(loop && IsWorking);
    
    co_return;
}
//------------------------------------------------

task::CoroTaskVoid Session::AsyncWrite(buffer_ptr write_bf, engine::WorkerType type)
{
    auto write_buffer_view = std::as_bytes(std::span(*write_bf.get()));
    auto status = co_await Connection.async_write(Sheduler.get(), write_buffer_view, type);

    if(!status)
    {
        Close();
    }
    
    co_return;
}
//------------------------------------------------

void Session::Write(buffer_view_const write_bf)
{
    auto status = Connection.write(write_bf);

    if(!status)
    {
        Close();
    }

    return;
}
//------------------------------------------------

void Session::Close() noexcept
{
    LOG(Session, "Close session");
    if(Serv)
        Serv->CloseSession(Connection.Desc());
}
//------------------------------------------------

void Session::SendResult(const String& response)
{
    auto write_buffer_view = std::as_bytes(std::span(response));
    Write(write_buffer_view);
}
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
}