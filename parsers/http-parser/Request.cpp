#include "Request.h"
#include "HttpUtils.h"
namespace http{
//-----------------------------------------------------------

void Request::AddHeader(const String& header, const String& value) noexcept
{
    Headers.emplace(header, value);
}
//-----------------------------------------------------------

void Request::AddHeader(const char* start, const char* colon, const char* end)
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

    // if(header.length() == 6 && header == "cookie")
    // {

    // }
    // else
    // {
        
    // }

    Headers.emplace(std::move(header), std::move(value));
}
//-----------------------------------------------------------

void Request::SetBody(const String& body) noexcept
{
    Body = body;
}
//-----------------------------------------------------------

void Request::SetMethod(MethodType method) noexcept
{
    Method = method;
}
//-----------------------------------------------------------

void Request::SetUri(const String& uri) noexcept
{
    Uri = uri;
}
//-----------------------------------------------------------

bool Request::SetMethod(StringView method) noexcept
{
    Method = MethodFromString(method);

    return Method != Unknown;
}
//-----------------------------------------------------------

void Request::SetVersion(Version version) noexcept
{
    mVersion = version;
}
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
}