#pragma once
#include "ChainableTask.h"
#include "Listener.h"
#include "Sheduler.h"
#include "MetaType.h"
#include "ApiCommandBase.h"
namespace engine{
void RegisterAllMetaClass();
}
namespace io{
DECLARE_SHARED_PTR(Server)
DECLARE_WEAK_PTR(Server)

struct ServerConfiguration
{
    String host;
    uint port;
    uint SizeBytesInPacket;
};

struct RouteNil{};

template<class ConfParser, class ApiParser, class Route = RouteNil>
ServerSharedPtr CreateServer(ConfParser& parser, task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr worker_mediator, Route r = Route(),
                            uint SizeBytesInPacket = 1024);

template<class ConfParser>
ServerSharedPtr CreateServer(ConfParser& parser, task::ShedulerSharedPtr sheduler, 
                        handler_packet_f handler, engine::RegisterMediatorBasePtr worker_mediator, uint SizeBytesInPacket = 1024);

class Server
{
public:
    virtual ~Server() {}

    virtual task::CoroTaskVoid AsyncServerRun() = 0;

    virtual void SetArgs(task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr reg, const ServerConfiguration& configuration) = 0;

    virtual void CloseSession(const ID_t& id) noexcept = 0;

    template<class Parser, class Route = RouteNil>
    void InitDefaultHandler(Route r);

protected:
    template<class ConfParser, class ApiParser, class Route>
    friend ServerSharedPtr CreateServer(ConfParser& parser, task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr worker_mediator, Route r,
    uint SizeBytesInPacket);
    template<class ConfParser>
    friend ServerSharedPtr CreateServer(ConfParser& parser, task::ShedulerSharedPtr sheduler, 
    handler_packet_f handler, engine::RegisterMediatorBasePtr worker_mediator, uint SizeBytesInPacket);

    template<class ConfParser>
    static ServerSharedPtr CreateServer(ConfParser& parser, engine::RegisterMediatorBasePtr mediator, task::ShedulerSharedPtr sheduler, uint SizeBytesInPacket);

    engine::WorkerType Type;
    handler_packet_f HandlerPacket;
    ServerConfiguration Conf;
};

template<class ConfParser>
ServerSharedPtr Server::CreateServer(ConfParser& parser, engine::RegisterMediatorBasePtr mediator, task::ShedulerSharedPtr sheduler, uint SizeBytesInPacket)
{
    if(!mediator)
    {
        ERROR(Server, "mediator is null");
    }

    engine::RegisterAllMetaClass();
    auto server_opt = parser.template GetValue<typename ConfParser::BlockType>("Server");
    if(!server_opt.has_value())
    {
        ERROR(Server, "Config file is not contains Server");
        return nullptr;
    }
    ConfParser server_config(std::move(server_opt.value()));

    auto type_opt = server_config.template GetValue<String>("Type");
    if(!type_opt.has_value())
    {
        ERROR(Server, "Config file is not contains Server:Type");
        return nullptr;
    }

    ServerConfiguration conf;

    auto host_opt = server_config.template GetValue<String>("Host");

    if(!host_opt.has_value())
    {
        ERROR(Server, "Config file is not contains Host");
        return nullptr;
    }
    conf.host = std::move(host_opt.value());

    auto port_opt = server_config.template GetValue<uint>("Port");
    if(!port_opt.has_value())
    {
        ERROR(Server, "Config file is not contains Port");
        return nullptr;
    }
    conf.port = port_opt.value();

    conf.SizeBytesInPacket = SizeBytesInPacket;

    auto args = engine::Arguments<Server>(sheduler, mediator, conf);
    ServerSharedPtr server = std::static_pointer_cast<Server>(engine::MetaData::GetMetaData()->Create(type_opt.value(), &args));

    return server;
}

template<class ConfParser, class ApiParser, class Route>
ServerSharedPtr CreateServer(ConfParser& parser, task::ShedulerSharedPtr sheduler, engine::RegisterMediatorBasePtr worker_mediator, Route r,
uint SizeBytesInPacket)
{
    auto server = Server::CreateServer(parser, worker_mediator, sheduler, SizeBytesInPacket);
    server->template InitDefaultHandler<ApiParser, Route>(r);
    return server;
}

template<class ConfParser>
ServerSharedPtr CreateServer(ConfParser& parser, task::ShedulerSharedPtr sheduler,
                        handler_packet_f handler, engine::RegisterMediatorBasePtr worker_mediator, uint SizeBytesInPacket)
{
    auto server = Server::CreateServer(parser, worker_mediator, sheduler, SizeBytesInPacket);
    server->HandlerPacket = handler;
    return server;
}
//---------------------------------------------------------------

template<class Parser, class Route>
void Server::InitDefaultHandler(Route r)
{
    HandlerPacket = [r](buffer_view& read_buffer) mutable -> task::CoroTask<String>
    {
        String res;
        Parser parser;

        if constexpr (!std::is_same_v<Route, RouteNil>)
        {
            parser.SetRoute(r);
        }

        auto api_command = parser.ParseApiCommand(read_buffer);

        if(api_command)
        {
            api_command->template SetTools<api::DefaultTools<Parser>>({std::move(parser)});
            res = co_await api_command->ExecutionCommand();
        }
        else
        {
            ERROR(ApiCommand, "Command is not found");
        }
        co_return res;
    };
}
//---------------------------------------------------------------
}