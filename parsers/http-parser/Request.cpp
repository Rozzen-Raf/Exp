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

    if(header.length() == 6 && header == "cookie")
    {

    }
    else
    {
        switch(value.length())
        {
            case 6:
                if(header == "expect")
                {
                    Expect = value;
                }
                break;
        }
        Headers.emplace(std::move(header), std::move(value));
    }
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

void Request::ReserveBody(size_t contentLenght) noexcept
{
    Body.reserve(contentLenght);
}
//-----------------------------------------------------------

void Request::AppendToBody(const char* data, size_t count)
{
    //Simple impl
    Body.append(data, count);
}
//-----------------------------------------------------------

size_t Request::GetContentLenght() const noexcept
{
    return Body.length();
}
//-----------------------------------------------------------

void Request::RemoveHeader(const String& header)
{
    Headers.erase(header);
}
//-----------------------------------------------------------

const String& Request::GetHeader(const String& header) const noexcept
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
//-----------------------------------------------------------
//-----------------------------------------------------------
}