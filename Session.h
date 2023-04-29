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
    CoroTaskVoid AsyncRead(bool loop);
    CoroTaskVoid AsyncWrite(buffer write_bf);
    void Close() noexcept;

private:
    Socket Connection;
    ShedulerSharedPtr Sheduler;
    Server* Serv;
};
DECLARE_SHARED_PTR(Session)
#endif // SESSION_H
