#include "Headers.h"
#include "ApiCommandExample.h"
#include "HttpParser.h"
#include "JsonParser.h"
namespace api
{
    using namespace http;
    using namespace parse;
    METATYPE_DEF_TEMPL(Print<DefaultTools<HttpParser<JsonParser>>>)
    void RegisterMetaType()
    {
        REGISTER_METATYPE_TEMPL(Print, DefaultTools<HttpParser<JsonParser>>)
    }
}

http::RouteSharedPtr CreateRoute()
{
    auto route = std::make_shared<http::Route>();

    route->RegisterRoute("/print", engine::MetaData::GetMetaData()->GetMetaType("Print"));

    return route;
}

int main( int argc, char* argv[] ) 
{
    Catch::Session session;

    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;

    try{
        api::RegisterMetaType();
        using namespace engine;
		auto sheduler = GetSheduler();

		io::EpollWorkerSharedPtr worker = std::make_shared<io::EpollWorker>();
		sheduler->RegisterWorker(worker);

        String config = "{\
            \"Server\" : {\
                \"Host\" : \"127.0.0.1\",\
                \"Port\" : 1111,\
                \"Type\" : \"TcpServer\"\
            }\
		}";

        parse::JsonParser parser;
        auto buf = std::as_bytes(std::span(config));
        bool res = parser.Parse(buf);

        if(!res)
            return 1;

        RegisterMediatorBasePtr mediator = std::make_shared<RegisterMediator<io::EpollWorker>>(worker);
        auto server = io::CreateServer<parse::JsonParser, http::HttpParser<parse::JsonParser>>(parser, sheduler, mediator, CreateRoute());
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