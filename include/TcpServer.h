#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Server.h"
#include "MetaType.h"
#include "Session.h"
class TcpServer : public Server
{
    METATYPE
    using mutex_t = std::recursive_mutex;
    using lock_t = std::unique_lock<mutex_t>;
public:
    TcpServer();
    ~TcpServer();
    TcpServer(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const JsonParser& json_data);
    virtual void SetArgs(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const JsonParser& json_data) override;
    virtual CoroTaskVoid AsyncServerRun() final;
    virtual void CloseSession(const ID_t& id) noexcept final;

private:
    mutex_t mutex;
    std::unordered_map<int, Session> Sessions;
    Listener listener;
    IPEndPoint Addr;
    ShedulerSharedPtr Sheduler;
    std::atomic<bool> IsWorking;
    RegisterMediatorBasePtr Register;
};

#endif // TCPSERVER_H
