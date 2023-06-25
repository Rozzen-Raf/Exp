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
        ParseError,
        ParseCloseConnection
    };

    enum HttpStatusCode
    {
        kUnknown = 0,
        k101SwitchingProtocols = 101,
        k200Ok = 200,
        k204NoContent = 204,
        k400BadRequest = 400,
        k405MethodNotAllowed = 405,
        k413RequestEntityTooLarge = 413,
        k414RequestURITooLarge = 414,
        k417ExpectationFailed = 417,
        k501NotImplemented = 501,
    };

    enum ContentTypeCode
    {
        CT_NONE = 0,
    };

    struct Version
    {
        int VersionMajor = -1;
        int VersionMinor = -1;

        void Reset()
        {
            VersionMajor = -1;
            VersionMinor = -1;
        }

        String ToString() const noexcept
        {
            std::stringstream stream;
            stream << VersionMajor << '.' << VersionMinor;
            return stream.str();
        }
    };

    struct HttpParserResult
    {
        ParseResult result;
        String err_message;
        HttpStatusCode code;
    };
}