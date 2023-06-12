#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Server.h"
#include "Session.h"
namespace io
{
class TcpServer : public Server
{
    METATYPE
    using mutex_t = std::recursive_mutex;
    using lock_t = std::unique_lock<mutex_t>;
public:
    TcpServer();
    ~TcpServer();
    TcpServer(task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr reg,const ServerConfiguration& configuration);
    virtual void SetArgs(task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr reg, const ServerConfiguration& configuration) override;
    virtual task::CoroTaskVoid AsyncServerRun() final;
    virtual void CloseSession(const ID_t& id) noexcept final;

private:
    void InitHandler();

    mutex_t mutex;
    std::unordered_map<int, Session> Sessions;
    Listener listener;
    IPEndPoint Addr;
    task::ShedulerSharedPtr Sheduler;
    std::atomic<bool> IsWorking;
    engine::RegisterMediatorBasePtr Register;
};
}
#endif // TCPSERVER_H
