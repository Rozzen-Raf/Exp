#pragma once
#include "ChainableTask.h"
#include "JsonParser.h"
#include "Listener.h"
#include "Sheduler.h"
#include "MetaType.h"
#include "JsonUtils.h"

DECLARE_SHARED_PTR(Server)
DECLARE_WEAK_PTR(Server)

struct ServerConfiguration
{
    String host;
    uint port;
    uint SizeBytesInPacket;
};

template<class ConfParser, class ApiParser = ConfParser>
ServerSharedPtr CreateServer(ConfParser& parser,  RegisterMediatorBasePtr worker_mediator, ShedulerSharedPtr sheduler,
                            handler_packet_f handler = nullptr, uint SizeBytesInPacket = 256);

class Server
{
public:
    virtual ~Server() {}

    virtual CoroTaskVoid AsyncServerRun() = 0;

    virtual void SetArgs(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const ServerConfiguration& configuration) = 0;

    virtual void CloseSession(const ID_t& id) noexcept = 0;

    template<class Parser>
    void InitHandler();

protected:
    template<class ConfParser, class ApiParser>
    friend ServerSharedPtr CreateServer(ConfParser& parser,  RegisterMediatorBasePtr worker_mediator, ShedulerSharedPtr sheduler,
    handler_packet_f handler, uint SizeBytesInPacket);
    WorkerType Type;
    handler_packet_f HandlerPacket;
    ServerConfiguration Conf;
};
void RegisterAllMetaClass();
template<class ConfParser, class ApiParser>
ServerSharedPtr CreateServer(ConfParser& parser,  RegisterMediatorBasePtr worker_mediator, ShedulerSharedPtr sheduler, 
handler_packet_f handler, uint SizeBytesInPacket)
{
    RegisterAllMetaClass();
    auto server_opt = parser.template GetValue<typename ConfParser::BlockType>("Server");
    if(!server_opt.has_value())
    {
        ERROR(main, "Config file is not contains Server");
        return nullptr;
    }
    ConfParser server_config(std::move(server_opt.value()));

    auto type_opt = server_config.template GetValue<String>("Type");
    if(!type_opt.has_value())
    {
        ERROR(main, "Config file is not contains Server:Type");
        return nullptr;
    }

    ServerConfiguration conf;

    auto host_opt = server_config.template GetValue<String>("Host");

    if(!host_opt.has_value())
    {
        ERROR(TcpServer, "Config file is not contains Host");
        return nullptr;
    }
    conf.host = std::move(host_opt.value());

    auto port_opt = server_config.template GetValue<uint>("Port");
    if(!port_opt.has_value())
    {
        ERROR(TcpServer, "Config file is not contains Port");
        return nullptr;
    }
    conf.port = port_opt.value();

    conf.SizeBytesInPacket = SizeBytesInPacket;

    auto args = Arguments<Server>(sheduler, worker_mediator, conf);
    ServerSharedPtr server = std::static_pointer_cast<Server>(MetaData::GetMetaData()->Create(type_opt.value(), &args));
    if(handler)
        server->HandlerPacket = handler;
    else
        server->InitHandler<ApiParser>();
    return server;
}
//---------------------------------------------------------------
template<class Parser>
void Server::InitHandler()
{
    HandlerPacket = [](buffer_view& read_buffer) -> CoroTask<String>
    {
        String res;
        auto api_command_pair = ParseApiCommand<Parser>(read_buffer);

        if(api_command_pair.first)
        {
            if(ApiCommandWithTools<Parser>* p = dynamic_cast<ApiCommandWithTools<Parser>*>(api_command_pair.first.get()))
            {
                p->SetParser(std::move(api_command_pair.second));
            }
            res = api_command_pair.first->ExecutionCommand();
        }
        co_return res;
    };
}
//---------------------------------------------------------------