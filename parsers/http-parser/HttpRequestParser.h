#pragma once

#include "Types.h"
#include "HttpTypes.h"
#include "HttpUtils.h"
#include "Request.h"
#include "BufferHelper.h"

namespace http
{
    struct HttpParserResult
    {
        ParseResult result;
        String err_message;
    };

    class HttpRequestParser
    {        
    public:
        enum HttpRequestParserStatus
        {
            kMethod,
            kRequestLine,
            kHeaders,
            kBody,
            kReady
        };

        HttpParserResult parse(Request& request, engine::BufferHelper buffer);
    private:
        bool ProcessRequestLine(Request& request, StringView request_line);

        HttpRequestParserStatus status = kMethod;
    };    
}