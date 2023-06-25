#include "Response.h"
#include "HttpUtils.h"
namespace http{
//-----------------------------------------------------------

void Response::AddHeader(const String& header, const String& value) noexcept
{
    Headers.emplace(header, value);
}
//-----------------------------------------------------------

void Response::AddHeader(const char* start, const char* colon, const char* end)
{
    String header(start, colon);

    std::transform(header.begin(), header.end(), header.begin(), [](uint8_t c) { return tolower(c);});

    ++colon;
    while(colon < end && isspace(*colon))
    {
        ++colon;
    }

    String value(colon, end);
    while(!value.empty() && isspace(value[value.size() - 1]))
    {
        value.resize(value.size() - 1);
    }

    if(header.length() == 6 && header == "cookie")
    {

    }
    else
    {
        Headers.emplace(std::move(header), std::move(value));
    }
}
//-----------------------------------------------------------

void Response::SetBody(const String& body) noexcept
{
    Body = body;
}
//-----------------------------------------------------------

void Response::SetContentType(ContentTypeCode content_type) noexcept
{
    mContentType = content_type;
}
//-----------------------------------------------------------

void Response::SetStatusCode(HttpStatusCode status) noexcept
{
    mStatusCode = status;
}
//-----------------------------------------------------------

void Response::SetVersion(Version version) noexcept
{
    mVersion = version;
}
//-----------------------------------------------------------

void Response::ReserveBody(size_t contentLenght) noexcept
{
    Body.reserve(contentLenght);
}
//-----------------------------------------------------------

void Response::AppendToBody(const char* data, size_t count)
{
    //Simple impl
    Body.append(data, count);
}
//-----------------------------------------------------------

size_t Response::GetContentLenght() const noexcept
{
    return Body.length();
}
//-----------------------------------------------------------

void Response::RemoveHeader(const String& header)
{
    Headers.erase(header);
}
//-----------------------------------------------------------

const String& Response::GetHeader(const String& header) const noexcept
{
    const static String defaulValue;
    auto find_iter = Headers.find(header);
    if(find_iter != Headers.end())
    {
        return find_iter->second;
    }
    return defaulValue;
}
//-----------------------------------------------------------

const String& Response::GetBody() const noexcept
{
    return Body;
}
//-----------------------------------------------------------

Version Response::GetVersion() const noexcept
{
    return mVersion;
}
//-----------------------------------------------------------

HttpStatusCode Response::GetStatusCode() const noexcept
{
    return mStatusCode;
}
//-----------------------------------------------------------

ContentTypeCode Response::GetContentType() const noexcept
{
    return mContentType;
}
//-----------------------------------------------------------

void Response::Reset()
{
    mContentType = CT_NONE;
    mStatusCode = kUnknown;
    mVersion.Reset();
    Headers.clear();
    Body.clear();  
}
//-----------------------------------------------------------

void Response::SetStatus(const String& status) noexcept
{
    Status = status;
}
//-----------------------------------------------------------

const String& Response::GetStatus() const noexcept
{
    return Status;
}
//-----------------------------------------------------------

String Response::ToMessage() const noexcept
{
    std::stringstream stream;

    stream << "HTTP/" << mVersion.ToString() << ' ' << mStatusCode << ' ' << Status << "\r\n";

    for(auto header : Headers)
    {
        stream << header.first << ": " << header.second << "\r\n";
    }

    if(Body.length())
    {
        auto content_size = GetHeader("content-length");

        if(content_size.empty())
        {
            stream << "content-length" << ": " << Body.length() << "\r\n";
        }
    }

    stream << Body << "\r\n";
    return stream.str();
}
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
}