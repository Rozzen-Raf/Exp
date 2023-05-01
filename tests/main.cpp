#include "catch2/catch_all.hpp"
#include "ThreadPool.h"
#include "Sheduler.h"
#include "JsonParser.h"
#include "MetaTypeInclude.h"
#include "ChainableTask.h"
#include "EpollWorker.h"

int main( int argc, char* argv[] ) 
{
    Catch::Session session;

    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;

    try{
        RegisterAllMetaClass();
		ProcessorSharedPtr processor = std::make_shared<TaskProcessorModel<ThreadPool>>(8);
		ShedulerSharedPtr sheduler = std::make_shared<Sheduler>(processor);

		EpollWorkerSharedPtr worker = std::make_shared<EpollWorker>();
		sheduler->RegisterWorker(worker);

        String config = "{\
            \"Server\" : {\
                \"Host\" : \"127.0.0.1\",\
                \"Port\" : 11111,\
                \"Type\" : \"TcpServer\",\
            }\
		}";

        JsonParser parser;
        auto buf = std::as_bytes(std::span(config));
        bool res = parser.Parse(buf);

        if(!res)
            return 1;

        auto server_opt = parser.GetValue<json>("Server");
        if(!server_opt.has_value())
        {
            ERROR(main, "Config file is not contains Server");
        }
        JsonParser server_config(std::move(server_opt.value()));

        auto type_opt = server_config.GetValue<String>("Type");
        if(!type_opt.has_value())
        {
            ERROR(main, "Config file is not contains Server:Type");
        }

        RegisterMediatorBasePtr mediator = std::make_shared<RegisterMediator<EpollWorker>>(worker);

        auto args = Arguments<Server>(sheduler, mediator, server_config);
        ServerSharedPtr server = std::static_pointer_cast<Server>(MetaData::GetMetaData()->Create(type_opt.value(), &args));
        sheduler->CoroStart(server->AsyncServerRun());
		sheduler->Run();
	}
	catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception.\n";
    }

    return session.run();
}