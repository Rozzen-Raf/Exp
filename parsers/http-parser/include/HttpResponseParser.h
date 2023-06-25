#pragma once

#include "Response.h"
#include "BufferHelper.h"

namespace http
{
    class HttpResponseParser
    {
    public:
        enum HttpResponseParseStatus
        {
            kReponseLine,
            kHeaders,
            kBody,
            kChunkLen,
            kChunkBody,
            kLastEmptyChunk,
            kClose,
            kReady
        };
        HttpParserResult parse(Response& response, engine::BufferHelper buffer);
        bool ProcessResponseLine(Response& responce, StringView responce_line);
    private:
        HttpResponseParseStatus status;
        size_t contentSize = 0;
        size_t chunkSize = 0;
    };
}
