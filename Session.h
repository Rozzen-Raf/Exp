#ifndef SESSION_H
#define SESSION_H

#include "Socket.h"
#include "Sheduler.h"
class Session
{
public:
    Session(Socket&& connection, ShedulerSharedPtr sheduler);
    CoroTaskVoid AsyncRead();
private:
    Socket Connection;
    ShedulerSharedPtr Sheduler;
};
#endif // SESSION_H
