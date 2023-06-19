#include "HttpRequestParser.h"

namespace http
{
    ParseResult HttpRequestParser::parse(Request& request, engine::BufferHelper buffer)
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
                        return ParseError;
                    }

                    if(!request.SetMethod(StringView(buffer.Peek(), find_space - buffer.Peek())))
                    {
                        return ParseError;
                    }
                    status = kRequestLine;
                    buffer.MoveTo(find_space + 1);
                    break;
                }
            }
        }
    }
}