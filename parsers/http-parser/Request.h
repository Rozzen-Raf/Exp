#pragma once

#include "Types.h"
#include "HttpTypes.h"
namespace http
{
    class Request
    {
    public:
        void AddHeader(const String& header, const String& Value) noexcept;
        void SetMethod(MethodType method) noexcept;
        bool SetMethod(StringView method) noexcept;
        void SetUri(const String& uri) noexcept;
        void SetVersion(Version version) noexcept;
        void SetBody(const String& body) noexcept;

    private:
        MethodType Method;
        String Uri;
        Version mVersion;
        std::map<String, String> Headers;
        String Body;  
    };
}