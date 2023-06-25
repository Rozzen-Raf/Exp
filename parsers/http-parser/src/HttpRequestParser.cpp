#include "HttpRequestParser.h"

namespace http
{
    static constexpr size_t PacketSize = 64 * 1024;

    static constexpr size_t CRLF_LEN = 2;
    static constexpr size_t TRUNK_LEN_MAX_LEN = 16;
    HttpParserResult HttpRequestParser::parse(Request& request, engine::BufferHelper buffer)
    {      
        while(true)
        {
            switch(status)
            {
                case kMethod:
                {
                    auto* find_space = std::find(buffer.Peek(), buffer.end(), ' ');

                    if(find_space == buffer.end())
                    {
                        return {ParseError, "Don't find method from request", k400BadRequest};
                    }

                    if(!request.SetMethod(StringView(buffer.Peek(), find_space - buffer.Peek())))
                    {
                        return {ParseError, "Method is unknown", k405MethodNotAllowed};
                    }
                    status = kRequestLine;
                    buffer.MoveTo(find_space + 1);
                    continue;
                }
                case kRequestLine:
                {
                    auto crlf = buffer.FindCRLF();
                    if(!crlf)
                    {
                        return {ParseUncompleted, "Don't find request line from request", kUnknown};    
                    }
                    
                    if(!ProcessRequestLine(request, StringView(buffer.Peek(), crlf)))
                    {
                        return {ParseError, "Bad request", k400BadRequest};
                    }

                    buffer.MoveTo(crlf + CRLF_LEN);
                    status = kHeaders;
                    continue;
                }
                case kHeaders:
                {
                    auto crlf = buffer.FindCRLF();
                    if(!crlf)
                    {
                        return {ParseError, "Don't find headers", k400BadRequest};
                    }

                    auto colon = std::find(buffer.Peek(), crlf, ':');
                    if(colon != crlf)
                    {
                        request.AddHeader(buffer.Peek(), colon, crlf);
                        buffer.MoveTo(crlf + CRLF_LEN);
                        continue;
                    }

                    buffer.MoveTo(crlf + CRLF_LEN);

                    auto& len = request.GetHeader("content-length");
                    if(!len.empty())
                    {
                        try
                        {
                            contentLenght = static_cast<size_t>(std::stoull(len));
                        }
                        catch(...)
                        {
                            return {ParseError, "Content-lenght is invalid", k400BadRequest};
                        }

                        if(contentLenght == 0)
                        {
                            status = kReady;
                            return {ParseCompleted, "", k200Ok};
                        }      
                        else
                        {
                            status = kBody;
                        }
                    }
                    else
                    {
                        auto& encode = request.GetHeader("transfer-encoding");

                        if(encode.empty())
                        {
                            status = kReady;
                            return {ParseCompleted, "", k200Ok};
                        }
                        else if(encode == "chunked")
                        {
                            status = kChunkLen;
                        }
                        else
                        {
                            return {ParseError, "Unknown transfer-encoding", k501NotImplemented};
                        }
                    }

                    //auto& expect = request.
                    request.ReserveBody(contentLenght);
                    continue;
                }
                case kBody:
                {
                    size_t bytesToConsume =
                    contentLenght <= buffer.ReadableBytes()
                        ? contentLenght
                        : buffer.ReadableBytes();

                    if(bytesToConsume)
                    {
                        request.AppendToBody(buffer.Peek(), bytesToConsume);
                        buffer.MoveOn(bytesToConsume);
                        contentLenght -= bytesToConsume;
                    }

                    if(contentLenght == 0)
                    {
                        status = kReady;
                        return {ParseCompleted, "", k200Ok};
                    }
                    continue;
                }
                case kChunkLen:
                {
                    auto crlf = buffer.FindCRLF();
                    if(!crlf)
                    {
                        if(buffer.ReadableBytes() > TRUNK_LEN_MAX_LEN + CRLF_LEN)
                        {
                            return {ParseError, "", k400BadRequest};
                        }
                        return {ParseUncompleted, "", kUnknown};
                    }

                    String len(buffer.Peek(), crlf - buffer.Peek());
                    char* end;
                    chunkLength = strtol(len.c_str(), &end, 16);
                    if(chunkLength != 0)
                    {
                        status = kChunkBody;
                    }
                    else{
                        status = kLastEmptyChunk;
                    }
                    buffer.MoveTo(crlf + CRLF_LEN);
                    continue;
                }
                case kChunkBody:
                {
                    if(buffer.ReadableBytes() <  (chunkLength + CRLF_LEN))
                    {
                        return {ParseUncompleted, "", kUnknown};
                    }
                    if(*(buffer.Peek() + chunkLength) != '\r' ||
                       *(buffer.Peek() + chunkLength + 1) != 'n' )
                    {
                        return {ParseError, "Bad chunk parsing", k400BadRequest};
                    }

                    request.AppendToBody(buffer.Peek(), chunkLength);
                    buffer.MoveOn(chunkLength + CRLF_LEN);
                    contentLenght += chunkLength;
                    chunkLength = 0;
                    status = kChunkLen;
                    continue;
                }
                case kLastEmptyChunk:
                {
                    if(buffer.ReadableBytes() < CRLF_LEN)
                    {
                        return {ParseUncompleted, "", kUnknown};
                    }

                    if(*(buffer.Peek()) != '\r' || *(buffer.Peek() + 1) != '\n')
                    {
                        return {ParseError, "Last empty chunk parse error", k400BadRequest};
                    }
                    buffer.MoveOn(CRLF_LEN);
                    status = kReady;
                    request.AddHeader("content-length", std::to_string(request.GetContentLenght()));
                    request.RemoveHeader("transfer-encoding");
                    return {ParseCompleted, "", k200Ok};
                }
                case kReady:
                {
                    return {ParseCompleted, "", k200Ok};
                }
            }
        }

        return {ParseUncompleted, "", kUnknown};
    }

    bool HttpRequestParser::ProcessRequestLine(Request& request, StringView request_line)
    {
        bool res = false;
        // const char* start = request_line.data();
        // const char* end = request_line.data() + request_line.size();

        auto space = request_line.find(' ');
        if(space != StringView::npos)
        {
            request.SetUri(String(request_line.data(), space));
        }

        auto http = request_line.find("HTTP/");
        res = request_line.size() - space - 1 == 8 && http != StringView::npos;
        if(res)
        {
            const char* start = request_line.data() + http + 5;
            int versionMajor = -1;
            int versionMinor = -1;
            if(isdigit(*start))
            {
                versionMajor = *start - '0';
                start += 2;
                if(isdigit(*start))
                {
                    versionMinor = *start - '0';
                }
            }

            res = versionMajor != -1 && versionMinor != -1;
            if(res)
            {
                request.SetVersion({versionMajor, versionMinor});
            }
        }
        return res;
    }
}