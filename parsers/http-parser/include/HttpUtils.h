#pragma once

#include "Types.h"
#include "HttpTypes.h"

namespace http
{
    MethodType MethodFromString(StringView str);
    String MethodToString(MethodType method);
}