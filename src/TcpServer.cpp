#include "TcpServer.h"
//--------------------------------------------------------
METATYPE_DEF(TcpServer)
//--------------------------------------------------------
TcpServer::TcpServer(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const JsonParser& json_data) : 
listener(nullptr),  
IsWorking(true)
{
    SetArgs(sheduler, reg, json_data);
}
//--------------------------------------------------------

TcpServer::TcpServer() : 
listener(nullptr), 
IsWorking(true)
{
}
//--------------------------------------------------------

TcpServer::~TcpServer()
{
    LOG(TcpServer, "destroy");
    IsWorking = false;
    Sessions.clear();
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
        auto status = co_await listener.AsyncAccept(Sheduler);

        if(!IsWorking)
            co_return;

        if(!status)
        {
            ERROR(TcpServer, status.err_message);
            co_return;
        }

        if(!std::holds_alternative<ID_t>(status.id))
            continue;

        {
            lock_t l(mutex);
            ID_t id = std::get<ID_t>(status.id);
            Socket s(IPv::IPv4, id);
            auto session_iter = Sessions.emplace(id, Session(this, std::move(s), Sheduler));

            if(session_iter.second)
            {
                Sheduler->CoroStart(session_iter.first->second.AsyncRead(true));
            }
        }
    }    

    co_return;
}
//--------------------------------------------------------

void TcpServer::SetArgs(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const JsonParser& json_data)
{
    ASSERT(sheduler);

    Sheduler = sheduler;
    Register = reg;
    listener.SetRegister(reg);

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

void TcpServer::CloseSession(const ID_t &id) noexcept
{
    lock_t l(mutex);
    Sessions.erase(id);
}
//--------------------------------------------------------

void TcpServer::RedirectAll(const ID_t &id, buffer_ptr buf) noexcept
{
    lock_t l(mutex);
    for(auto&& conn : Sessions)
    {
       if(conn.first == id)
           continue;
       Sheduler->CoroStart(conn.second.AsyncWrite(buf));
    }
}
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------

