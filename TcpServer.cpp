#include "TcpServer.h"
//--------------------------------------------------------
TcpServer::TcpServer(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr register, const json& json_data) : 
listener(nullptr), 
Sheduler(sheduler), 
IsWorking(true),
Register(register)
{
    ASSERT(sheduler);

    String host = json_data.GetValue<String>("Host");
    uint port = json_data.GetValue<uint>("Port");

    Addr = IPEndPoint(host.c_str(), port);
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
        Socket s = std::move(co_await listener.AsyncAccept());
    }    

    co_return;
}
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------

