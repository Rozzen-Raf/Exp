#ifndef SESSION_H
#define SESSION_H

#include "Socket.h"
#include "Sheduler.h"
#include "Server.h"

namespace io{

class Session
{
public:
    Session(Server* serv, Socket&& connection, task::ShedulerSharedPtr sheduler) noexcept;
    Session(const Session& s) = delete;
    Session(Session&& s);
    ~Session();
    task::CoroTaskVoid AsyncRead(bool loop, engine::WorkerType type, uint size_buffer, handler_packet_f handler_packet);
    task::CoroTaskVoid AsyncWrite(buffer_ptr write_bf, engine::WorkerType type);

    void Write(buffer_view_const write_bf);

    void Close() noexcept;

    void SendResult(const String& response);

private:
    Socket Connection;
    task::ShedulerSharedPtr Sheduler;
    Server* Serv;
    std::atomic<bool> IsWorking;
};
DECLARE_SHARED_PTR(Session)
}
#endif // SESSION_H
