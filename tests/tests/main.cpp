#include "Headers.h"
int main( int argc, char* argv[] ) 
{
    Catch::Session session;

    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;

    try{
		auto sheduler = GetSheduler();

		EpollWorkerSharedPtr worker = std::make_shared<EpollWorker>(OnlyByID);
		sheduler->RegisterWorker(worker);

        String config = "{\
            \"Server\" : {\
                \"Host\" : \"127.0.0.1\",\
                \"Port\" : 11111,\
                \"Type\" : \"TcpServer\"\
            }\
		}";

        JsonParser parser;
        auto buf = std::as_bytes(std::span(config));
        bool res = parser.Parse(buf);

        if(!res)
            return 1;

        RegisterMediatorBasePtr mediator = std::make_shared<RegisterMediator<EpollWorker>>(worker);
        auto server = CreateServer(parser, mediator, sheduler);
        if(!server)
        {
            throw std::runtime_error("server not created");
        }

        sheduler->CoroStart(server->AsyncServerRun());
		sheduler->Run(false);
        return session.run();
	}
	catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception.\n";
    }
    return 1;
}