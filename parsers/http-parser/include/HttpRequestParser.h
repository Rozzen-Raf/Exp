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
            kBody,
            kChunkLen,
            kChunkBody,
            kLastEmptyChunk,
            kReady
        };

        HttpParserResult parse(RequestImpl& request, engine::BufferHelper buffer);
    private:
        bool ProcessRequestLine(RequestImpl& request, StringView request_line);

        HttpRequestParserStatus status = kMethod;
        size_t contentLenght = 0;
        size_t chunkLength = 0;
    };    
}