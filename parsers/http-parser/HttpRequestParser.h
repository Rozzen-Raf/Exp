#pragma once

#include "Types.h"
#include "HttpTypes.h"
#include "HttpUtils.h"
#include "Request.h"
#include "BufferHelper.h"

namespace http
{
    class HttpRequestParser
    {        
    public:
        enum HttpRequestParserStatus
        {
            kMethod,
            kRequestLine,
            kHeaders,
            kBody
        };

        ParseResult parse(Request& request, engine::BufferHelper buffer);
    private:
        HttpRequestParserStatus status = kMethod;
    };    
}