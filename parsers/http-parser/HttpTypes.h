#pragma once

namespace http
{
    enum MethodType
    {
        Unknown,
        GET,
        POST,
        PUT,
        DELETE,
        PATCH,
        All
    };

    enum ParseResult
    {
        ParseCompleted,
        ParseUncompleted,
        ParseError
    };

    struct Version
    {
        int VersionMajor;
        int VersionMinor;
    };
}