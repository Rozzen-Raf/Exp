#include "TcpServer.h"
namespace io
{
//--------------------------------------------------------
METATYPE_DEF(TcpServer)
//--------------------------------------------------------
TcpServer::TcpServer(task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr reg, const ServerConfiguration& conf) :
listener(nullptr),  
IsWorking(true)
{
    LOG(TcpServer, "Create");
    SetArgs(sheduler, reg, conf);
}
//--------------------------------------------------------

TcpServer::TcpServer() : 
listener(nullptr), 
IsWorking(true)
{
    LOG(TcpServer, "Create");
}
//--------------------------------------------------------

TcpServer::~TcpServer()
{
    LOG(TcpServer, "destroy");
    IsWorking = false;
    Sessions.clear();
    Register->Unregister(listener.Desc());
}
//--------------------------------------------------------

task::CoroTaskVoid TcpServer::AsyncServerRun()
{
    if(!listener.Create())
        co_return;

    Register->Register(listener.Desc());

    if(!listener.Listen(Addr))
        co_return;

    //     Sheduler->CoroStart(AsyncSessionHandle());
   while(IsWorking)
   {
       auto status = co_await listener.AsyncAccept(Sheduler, Type);

       if(!IsWorking)
           co_return;

       if(!status)
       {
           ERROR(TcpServer, status.err_message);
           co_return;
       }

       {
           lock_t l(mutex);
           ID_t id = status.id;
           Socket s(IPv::IPv4, id);
           auto session_iter = Sessions.emplace(id, Session(this, std::move(s), Sheduler));

           if(session_iter.second)
           {
               Sheduler->CoroStart(session_iter.first->second.AsyncRead(true, Type, Conf.SizeBytesInPacket, HandlerPacket));
           }
       }
   }

    co_return;
}
//------------------------------------------------------------

void TcpServer::SetArgs(task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr reg, const ServerConfiguration& conf)
{
    ASSERT(sheduler);

    Sheduler = sheduler;
    Register = reg;
    Conf = conf;
    Type = static_cast<engine::WorkerType>(Register->Type());
    listener.SetRegister(reg);

    const String& host = conf.host;
    uint port = conf.port;

    Addr = IPEndPoint(host.c_str(), port);
}
//--------------------------------------------------------

void TcpServer::CloseSession(const ID_t &id) noexcept
{
    lock_t l(mutex);
    Sessions.erase(id);
    Register->Unregister(id);
}
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
}
