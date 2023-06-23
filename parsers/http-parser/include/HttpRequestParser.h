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
        ResponseCode code;
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
            kChunkLen,
            kChunkBody,
            kLastEmptyChunk,
            kReady
        };

        HttpParserResult parse(Request& request, engine::BufferHelper buffer);
    private:
        bool ProcessRequestLine(Request& request, StringView request_line);

        HttpRequestParserStatus status = kMethod;
        size_t contentLenght = 0;
        size_t chunkLength = 0;
    };    
}