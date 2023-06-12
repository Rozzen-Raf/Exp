﻿#include "ThreadPool.h"
#include "Sheduler.h"
#include "JsonParser.h"
#include "ApiCommandExample.h"
#include "MetaType.h"
#include "ChainableTask.h"
#include "EpollWorker.h"
#include <cstddef>
#include "Socket.h"
#include "TcpServer.h"
using namespace parse;
namespace api
{
    METATYPE_DEF_TEMPL(Print<JsonParser>)
    void RegisterMetaType()
    {
        REGISTER_METATYPE_TEMPL(Print, JsonParser)
    }
}
void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here  
   // terminate program  

   exit(signum);  
}
using namespace engine;
int main()
{
    signal(SIGINT, signalHandler);

	try{
        api::RegisterMetaType();
        using CoroPool = task::ThreadPool<>;
        task::ProcessorSharedPtr processor = std::make_shared<task::TaskProcessorModel<CoroPool>>(8);
		task::ShedulerSharedPtr sheduler = std::make_shared<task::Sheduler>(processor);

		io::EpollWorkerSharedPtr worker = std::make_shared<io::EpollWorker>(OnlyByID);
		sheduler->RegisterWorker(worker);

        RegisterMediatorBasePtr mediator = std::make_shared<RegisterMediator<io::EpollWorker>>(worker);

        JsonParser parser;
        parser.ParseFromFile("config.json");

        auto server = io::CreateServer(parser, mediator, sheduler);
        if(!server)
        {
            throw std::runtime_error("server not created");
        }
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
	return 0;
}
