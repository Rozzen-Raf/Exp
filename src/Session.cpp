#include "Session.h"
//------------------------------------------------

Session::Session(Server* serv, Socket&& connection, ShedulerSharedPtr sheduler) noexcept : Connection(std::move(connection)), Sheduler(sheduler), Serv(serv)
{

}
//------------------------------------------------

Session::Session(Session&& right) : Connection(std::move(right.Connection)), Sheduler(std::exchange(right.Sheduler, nullptr)), Serv(std::exchange(right.Serv, nullptr))
{

}
//------------------------------------------------
Session::~Session()
{
    IsWorking = false;
}
//------------------------------------------------

CoroTaskVoid Session::AsyncRead(bool loop)
{
    IsWorking = loop;
    
    do{
        buffer_ptr read_buffer = std::make_shared<buffer>(256);
        auto status = co_await Connection.async_read(Sheduler, read_buffer);

        if(!IsWorking)
            co_return;

        if(!status)
        {
            loop = false;
            ERROR(Session, status.err_message);
            Close();
            co_return;
        }
        Serv->RedirectAll(Connection.Desc(), read_buffer);

//        auto api_command_pair = ParseJsonApiCommand(read_buffer);

//        std::pair<ID_t, Result> res{-1, Result::UnknownCommand};
//        if(api_command_pair.first)
//        {
//            res = api_command_pair.first->ExecutionCommand(JsonParser(std::move(api_command_pair.second)));
//        }

//        SendResult(res.first, res.second);
    }while(loop && IsWorking);
    
    co_return;
}
//------------------------------------------------

CoroTaskVoid Session::AsyncWrite(buffer_ptr write_bf)
{
    auto status = co_await Connection.async_write(Sheduler, write_bf);

    if(!status)
    {
        Close();
    }
    
    co_return;
}
//------------------------------------------------

void Session::Close() noexcept
{
    LOG(Session, "Close session");
    if(Serv)
        Serv->CloseSession(Connection.Desc());
}
//------------------------------------------------

void Session::SendResult(ID_t message_id, Result res)
{
    JsonParser parser;
    parser.SetValue("MessageID", std::move(message_id));
    parser.SetValue("ResultID", std::move(res));

    buffer_ptr buffer = ConvertStringToBuffer(parser.Dump());

    Sheduler->CoroStart(AsyncWrite(buffer));
}
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
