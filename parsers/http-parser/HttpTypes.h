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

    enum ResponseCode
    {
        kUnknown = 0,
        k200Ok = 200,
        k400BadRequest = 400,
        k405MethodNotAllowed = 405,
        k413RequestEntityTooLarge = 413,
        k414RequestURITooLarge = 414,
        k417ExpectationFailed = 417,
        k501NotImplemented = 501,
    };

    struct Version
    {
        int VersionMajor;
        int VersionMinor;
    };
}