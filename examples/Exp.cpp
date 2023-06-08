#include "ThreadPool.h"
#include "Sheduler.h"
#include "JsonParser.h"
#include "MetaTypeInclude.h"
#include "ChainableTask.h"
#include "EpollWorker.h"
#include <cstddef>
#include "Socket.h"

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
        RegisterAllMetaClass();
        using CoroPool = ThreadPool<std::coroutine_handle<>>;
		ProcessorSharedPtr processor = std::make_shared<TaskProcessorModel<ThreadPool<std::coroutine_handle<>>>>(8);
		ShedulerSharedPtr sheduler = std::make_shared<Sheduler>(processor);

		EpollWorkerSharedPtr worker = std::make_shared<EpollWorker>(OnlyByID);
		sheduler->RegisterWorker(worker);

        JsonParser parser;
        parser.ParseFromFile("config.json");

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
	return 0;
}
