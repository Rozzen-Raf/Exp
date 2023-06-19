#include "Request.h"
#include "HttpUtils.h"
namespace http{
//-----------------------------------------------------------

void Request::AddHeader(const String& header, const String& value) noexcept
{
    Headers.emplace(header, value);
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