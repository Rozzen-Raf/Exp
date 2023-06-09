#include "ThreadPool.h"
#include "Sheduler.h"
#include "JsonParser.h"
#include "MetaTypeInclude.h"
#include "ChainableTask.h"
#include "EpollWorker.h"
#include <cstddef>
#include "Socket.h"

METATYPE_DEF_TEMPL(Print<JsonParser>)
void RegisterMetaType()
{
    REGISTER_METATYPE_TEMPL(Print, JsonParser)
}

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here  
   // terminate program  

   exit(signum);  
}

int main()
{
    signal(SIGINT, signalHandler);

	try{
        using CoroPool = ThreadPool<>;
        ProcessorSharedPtr processor = std::make_shared<TaskProcessorModel<CoroPool>>(8);
		ShedulerSharedPtr sheduler = std::make_shared<Sheduler>(processor);

		EpollWorkerSharedPtr worker = std::make_shared<EpollWorker>(OnlyByID);
		sheduler->RegisterWorker(worker);

        RegisterMediatorBasePtr mediator = std::make_shared<RegisterMediator<EpollWorker>>(worker);

        JsonParser parser;
        parser.ParseFromFile("config.json");

        auto server = CreateServer(parser, mediator, sheduler);
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
