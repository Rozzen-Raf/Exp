#pragma once

#include "Types.h"
#include "HttpTypes.h"
namespace http
{
    class Request
    {
    public:
        void AddHeader(const String& header, const String& Value) noexcept;
        void AddHeader(const char* start, const char* colon, const char* end);
        void RemoveHeader(const String& header);
        void SetMethod(MethodType method) noexcept;
        bool SetMethod(StringView method) noexcept;
        void SetUri(const String& uri) noexcept;
        void SetVersion(Version version) noexcept;
        void SetBody(const String& body) noexcept;
        void AppendToBody(const char* data, size_t count);
        void ReserveBody(size_t contentLenght) noexcept;

        const String& GetHeader(const String& header) const noexcept;
        size_t GetContentLenght() const noexcept;
        MethodType GetMethod() const noexcept;
        const String& GetUri() const noexcept;
        Version GetVersion() const noexcept;
        const String& GetBody() const noexcept;

        void Reset();

    private:
        MethodType Method;
        String Uri;
        Version mVersion;
        std::map<String, String> Headers;
        String Body;  
        String Expect;
    };
}