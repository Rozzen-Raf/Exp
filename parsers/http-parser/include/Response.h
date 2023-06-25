#pragma once

#include "Types.h"
#include "HttpTypes.h"

namespace http
{
    class Response
    {
    public:
        void SetVersion(Version version) noexcept;
        void SetStatusCode(HttpStatusCode code) noexcept;
        void SetContentType(ContentTypeCode content_type) noexcept;
        void AddHeader(const String& header, const String& Value) noexcept;
        void AddHeader(const char* start, const char* colon, const char* end);
        void RemoveHeader(const String& header);
        void SetBody(const String& body) noexcept;
        void AppendToBody(const char* data, size_t count);
        void ReserveBody(size_t contentLenght) noexcept;
        void SetStatus(const String& status) noexcept;

        const String& GetHeader(const String& header) const noexcept;
        size_t GetContentLenght() const noexcept;
        HttpStatusCode GetStatusCode() const noexcept;
        ContentTypeCode GetContentType() const noexcept;
        Version GetVersion() const noexcept;
        const String& GetBody() const noexcept;
        const String& GetStatus() const noexcept;

        void Reset();

        String ToMessage() const noexcept;

    private:
        HttpStatusCode mStatusCode = kUnknown;
        Version mVersion;
        ContentTypeCode mContentType = CT_NONE;
        std::map<String, String> Headers;
        String Body;
        String Status;
        bool CloseConnection = false;
    };
}
