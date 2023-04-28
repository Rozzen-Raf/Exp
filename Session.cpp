#include "Session.h"
//------------------------------------------------

Session::Session(Socket&& connection, ShedulerSharedPtr sheduler) : Connection(std::move(connection)), Sheduler(sheduler)
{

}
//------------------------------------------------

CoroTaskVoid Session::AsyncRead()
{
    uint32_t rd_size[1] = {0};
    auto read_buffer = std::as_writable_bytes(std::span(rd_size));
    

    co_return;
}
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------