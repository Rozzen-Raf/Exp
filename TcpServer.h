#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Session.h"
#include "ChainableTask.h"
#include "JsonParser.h"
#include "Listener.h"
#include "Sheduler.h"

class TcpServer
{
    using mutex_t = std::recursive_mutex;
    using lock_t = std::unique_lock<mutex_t>;
public:
    TcpServer(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr register, const json& json_data);
    CoroTaskVoid AsyncServerRun();
private:
    mutex_t mutex;
    std::unordered_map<UID_t, Session> Sessions;
    Listener listener;
    IPEndPoint Addr;
    ShedulerSharedPtr Sheduler;
    std::atomic<bool> IsWorking;
    RegisterMediatorBasePtr Register;
};

#endif // TCPSERVER_H
