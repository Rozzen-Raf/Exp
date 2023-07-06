#include "Request.h"
#include "HttpUtils.h"
namespace http{
//-----------------------------------------------------------

void RequestImpl::AddHeader(const String& header, const String& value) noexcept
{
    Headers.emplace(header, value);
}
//-----------------------------------------------------------

void RequestImpl::AddHeader(const char* start, const char* colon, const char* end)
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

void RequestImpl::SetBody(const String& body) noexcept
{
    Body = body;
}
//-----------------------------------------------------------

void RequestImpl::SetMethod(MethodType method) noexcept
{
    Method = method;
}
//-----------------------------------------------------------

void RequestImpl::SetUri(const String& uri) noexcept
{
    Uri = uri;
}
//-----------------------------------------------------------

bool RequestImpl::SetMethod(StringView method) noexcept
{
    Method = MethodFromString(method);

    return Method != Unknown;
}
//-----------------------------------------------------------

void RequestImpl::SetVersion(Version version) noexcept
{
    mVersion = version;
}
//-----------------------------------------------------------

void RequestImpl::ReserveBody(size_t contentLenght) noexcept
{
    Body.reserve(contentLenght);
}
//-----------------------------------------------------------

void RequestImpl::AppendToBody(const char* data, size_t count)
{
    //Simple impl
    Body.append(data, count);
}
//-----------------------------------------------------------

size_t RequestImpl::GetContentLenght() const noexcept
{
    return Body.length();
}
//-----------------------------------------------------------

void RequestImpl::RemoveHeader(const String& header)
{
    Headers.erase(header);
}
//-----------------------------------------------------------

const String& RequestImpl::GetHeader(const String& header) const noexcept
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

MethodType RequestImpl::GetMethod() const noexcept
{
    return Method;
}
//-----------------------------------------------------------

const String& RequestImpl::GetUri() const noexcept
{
    return Uri;
}
//-----------------------------------------------------------

const String& RequestImpl::GetBody() const noexcept
{
    return Body;
}
//-----------------------------------------------------------

Version RequestImpl::GetVersion() const noexcept
{
    return mVersion;
}
//-----------------------------------------------------------
void RequestImpl::Reset()
{
    Method = MethodType::Unknown;
    Uri.clear();
    mVersion.Reset();
    Headers.clear();
    Body.clear();  
    Expect.clear();
}
//-----------------------------------------------------------

String RequestImpl::ToMessage() const noexcept
{
    std::stringstream stream;

    stream << Method << " " << Uri << " HTTP/"
               << mVersion.ToString() << "\r\n";

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