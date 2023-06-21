#include "HttpRequestParser.h"

namespace http
{
    static constexpr size_t PacketSize = 64 * 1024;

    static constexpr size_t CRLF_LEN = 2;
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
                        return {ParseError, "Don't find method from request"};
                    }

                    if(!request.SetMethod(StringView(buffer.Peek(), find_space - buffer.Peek())))
                    {
                        return {ParseError, "Method is unknown"};
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
                        return {ParseError, "Don't find request line from request"};    
                    }
                    
                    if(!ProcessRequestLine(request, StringView(buffer.Peek(), crlf)))
                    {
                        return {ParseError, "Bad request"};
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
                        return {ParseError, "Don't find headers"};
                    }

                    auto colon = std::find(buffer.Peek(), crlf, ':');
                    if(colon != crlf)
                    {
                        request.AddHeader(buffer.Peek(), colon, crlf);
                        buffer.MoveTo(crlf + CRLF_LEN);
                        continue;
                    }
                    continue;
                }
                case kBody:
                {
                    continue;
                }
                case kReady:
                {
                    return {ParseCompleted, ""};
                }
            }
        }

        return {ParseUncompleted, ""};
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