#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Session.h"

class TcpServer
{
    using mutex_t = std::recursive_mutex;
    using lock_t = std::unique_lock<mutex_t>;
public:
    TcpServer();
    void Run();
private:
    mutex_t mutex;
    std::unordered_map<UID_t, Session> Sessions;
    Socket Listener;
};

#endif // TCPSERVER_H
