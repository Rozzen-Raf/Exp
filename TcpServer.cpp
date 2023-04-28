#include "TcpServer.h"
//--------------------------------------------------------
METATYPE_DEF(TcpServer)
//--------------------------------------------------------
TcpServer::TcpServer(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const JsonParser& json_data) : 
listener(nullptr),  
IsWorking(true)
{
    ASSERT(sheduler);

    SetArgs(sheduler, reg, json_data);
}
//--------------------------------------------------------

TcpServer::TcpServer() : 
listener(nullptr), 
IsWorking(true)
{
}
//--------------------------------------------------------

CoroTaskVoid TcpServer::AsyncServerRun()
{
    if(!listener.Create())
        co_return;

    Register->Register(listener.Desc());

    if(!listener.Listen(Addr))
        co_return;

    while(IsWorking)
    {
        Socket s = std::move(co_await listener.AsyncAccept(Sheduler));
        Sessions.emplace(s.Desc(), Session(std::move(s), Sheduler));
    }    

    co_return;
}
//--------------------------------------------------------

void TcpServer::SetArgs(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const JsonParser& json_data)
{
    Sheduler = sheduler;
    Register = reg;

    auto host_opt = json_data.GetValue<String>("Host");

    if(!host_opt.has_value())
    {
        ERROR(TcpServer, "Config file is not contains Host");
        return;
    }
    String host = std::move(host_opt.value());

    auto port_opt = json_data.GetValue<uint>("Port");
    if(!port_opt.has_value())
    {
        ERROR(TcpServer, "Config file is not contains Port");
        return;
    }

    uint port = std::move(port_opt.value());

    Addr = IPEndPoint(host.c_str(), port);
}
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------

