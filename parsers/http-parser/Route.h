#pragma once

#include "Types.h"
#include "MetaType.h"

enum MethodType
{
    GET,
    POST,
    PUT,
    DELETE,
    All
};

class Route
{
    struct RouteItem
    {
        String Pattern;
        engine::MetaTypeSharedPtr MetaType;
        MethodType Method;
    };
public:
    void RegisterRoute(const String& pattern, engine::MetaTypeSharedPtr metatype, MethodType method = All);

private:
    std::vector<RouteItem> Routes;
};