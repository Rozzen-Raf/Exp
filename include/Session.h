#ifndef SESSION_H
#define SESSION_H

#include "Socket.h"
#include "Sheduler.h"
#include "Server.h"
#include "JsonUtils.h"
class Session
{
public:
    Session(Server* serv, Socket&& connection, ShedulerSharedPtr sheduler) noexcept;
    Session(const Session& s) = delete;
    Session(Session&& s);
    ~Session();
    CoroTaskVoid AsyncRead(bool loop);
    CoroTaskVoid AsyncWrite(buffer_ptr write_bf);
    void Close() noexcept;

    void SendResult(ID_t message_id, Result res);

private:
    Socket Connection;
    ShedulerSharedPtr Sheduler;
    Server* Serv;
    std::atomic<bool> IsWorking;
};
DECLARE_SHARED_PTR(Session)
#endif // SESSION_H
