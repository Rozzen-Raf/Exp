#include "HttpResponseParser.h"
namespace http
{
//-------------------------------------------------------------------

HttpParserResult HttpResponseParser::parse(ResponseImpl& response, engine::BufferHelper buffer)
{
    bool ok = true;
    bool hasMore = true;
    while(hasMore)
    {
        switch (status)
        {
        case kReponseLine:
        {
            auto crlf = buffer.FindCRLF();
            if(crlf)
            {
                ok = ProcessResponseLine(response, StringView(buffer.Peek(), crlf));
                if(ok)
                {
                    buffer.MoveTo(crlf + 2);
                    status = kHeaders;
                }
                else
                {
                    return {ParseError, "Response line parse error", kUnknown};
                }
            }
            else
            {
                return {ParseError, "Response is invalid", kUnknown};
            }
            continue;
        }
        case kHeaders:
        {
            auto crlf = buffer.FindCRLF();
            if(crlf)
            {
                auto colon = std::find(buffer.Peek(), crlf, ':');
                if(colon != crlf)
                {
                    response.AddHeader(buffer.Peek(), colon, crlf);
                }
                else
                {
                    const String& len = response.GetHeader("content-length");
                    if(!len.empty())
                    {
                        contentSize = static_cast<size_t>(std::stoull(len));
                        status = kBody;
                    }
                    else
                    {
                        const String& encode = response.GetHeader("transfer-encoding");
                        if(encode == "chunked")
                        {
                            status = kChunkLen;
                        }
                        else
                        {
                            if(response.GetStatusCode() == k204NoContent || (response.GetStatusCode() == k101SwitchingProtocols && [&response]()->bool{
                                String upgradeValue = response.GetHeader("upgrade");
                                std::transform(upgradeValue.begin(), upgradeValue.end(), upgradeValue.begin(),[](uint8_t c)
                                {
                                    return tolower(c);
                                });

                                return upgradeValue == "websocket";
                            }()))
                            {
                                status = kReady;
                                return {ParseCompleted, "", response.GetStatusCode()};
                            }
                            else
                            {
                                status = kClose;
                            }
                        }
                    }
                    buffer.MoveTo(crlf + 2);
                }
            }
            continue;
        }
        case kBody:
        {
            if(buffer.ReadableBytes() == 0)
            {
                if(contentSize == 0)
                {
                    status = kReady;
                    return {ParseCompleted, "", kUnknown};
                }
                break;
            }
            if(contentSize >= buffer.ReadableBytes())
            {
                contentSize -= buffer.ReadableBytes();

                response.AppendToBody(buffer.Peek(), buffer.ReadableBytes());
            }
            else
            {
                response.AppendToBody(buffer.Peek(), contentSize);
                buffer.MoveOn(contentSize);
                contentSize = 0;
            }
            if(contentSize == 0)
            {
                status = kReady;
                return {ParseCompleted, "", response.GetStatusCode()};
            }
            continue;
        }
        case kClose:
        {
            response.AppendToBody(buffer.Peek(), buffer.ReadableBytes());
            return {ParseCloseConnection, "", response.GetStatusCode()};
        }
        case kChunkLen:
        {
            auto crlf = buffer.FindCRLF();
            if(crlf)
            {
                StringView len(buffer.Peek(), crlf - buffer.Peek());
                char* end;
                chunkSize = strtol(len.data(), &end, 16);
                if(chunkSize != 0)
                {
                    status = kChunkBody;
                }
                else
                {
                    status = kLastEmptyChunk;
                }
                buffer.MoveTo(crlf + 2);
            }
            else
            {
                return {ParseError, "Chunk len parse error", response.GetStatusCode()};
            }
            continue;
        }
        case kChunkBody:
        {
            if(buffer.ReadableBytes() >= (chunkSize + 2))
            {
                if(*(buffer.Peek() + chunkSize) == '\r' && *(buffer.Peek() + chunkSize + 1) == '\n')
                {
                    response.AppendToBody(buffer.Peek(), chunkSize);
                    buffer.MoveOn(chunkSize + 2);
                    chunkSize = 0;
                    status = kChunkLen;
                }
                else
                {
                    return {ParseError, "Chunk parse error", response.GetStatusCode()};
                }
            }
            else
            {
                return {ParseError, "Chunk parse error", response.GetStatusCode()};
            }
            continue;
        }
        case kLastEmptyChunk:
        {
            auto crlf = buffer.FindCRLF();
            if(crlf)
            {
                buffer.MoveTo(crlf + 2);
                status = kReady;
                response.AddHeader("content-lenght", std::to_string(response.GetBody().length()));
                response.RemoveHeader("transfer-encoding");
                return {ParseCompleted, "", response.GetStatusCode()};
            }
            else
            {
                return {ParseError, "LastEmptyChunk parse error", response.GetStatusCode()};
            }
        }
        default:
            break;
        }
    }

    return {ParseUncompleted, "", kUnknown};
}
//-------------------------------------------------------------------

bool HttpResponseParser::ProcessResponseLine(ResponseImpl& response, StringView response_line)
{
    auto http = response_line.find("HTTP/");
    bool res = http != StringView::npos;
    if(res)
    {
        const char* version = response_line.data() + http + 5;
        int versionMajor = -1;
        int versionMinor = -1;
        if(isdigit(*version))
        {
            versionMajor = *version - '0';
            version += 2;
            if(isdigit(*version))
            {
                versionMinor = *version - '0';
            }
        }

        res = versionMajor != -1 && versionMinor != -1;
        if(res)
        {
            response.SetVersion({versionMajor, versionMinor});
        }

        auto start = http + 9;
        auto space = response_line.find(' ', start);
        res = space != StringView::npos;
        if(res)
        {
            StringView status_code(response_line.data() + start, space - start);
            StringView status_message(response_line.data() + space + 1, response_line.size() - space - 1);
            //LOG(HttpResponseParser, status_code + " " + status_message);
            auto code = atoi(status_code.data());
            response.SetStatusCode(HttpStatusCode(code));
        }
    }

    return res;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
}
