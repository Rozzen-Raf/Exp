#ifndef SESSION_H
#define SESSION_H

#include "Socket.h"
#include "Sheduler.h"
#include "Server.h"
class Session
{
public:
    Session(Server* serv, Socket&& connection, ShedulerSharedPtr sheduler) noexcept;
    Session(const Session& s) = delete;
    Session(Session&& s);
    ~Session();
    CoroTaskVoid AsyncRead(bool loop, WorkerType type, uint size_buffer, handler_packet_f handler_packet);
    CoroTaskVoid AsyncWrite(buffer_ptr write_bf, WorkerType type);

    void Write(buffer_view_const write_bf);

    void Close() noexcept;

    void SendResult(const String& response);

private:
    Socket Connection;
    ShedulerSharedPtr Sheduler;
    Server* Serv;
    std::atomic<bool> IsWorking;
};
DECLARE_SHARED_PTR(Session)
#endif // SESSION_H
