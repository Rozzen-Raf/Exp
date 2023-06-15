#pragma once

#include "Types.h"
#include "MetaType.h"
#include "ApiCommandBase.h"

namespace http
{
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
            std::vector<String> Pattern;
            engine::MetaTypeSharedPtr MetaType;
            MethodType Method;
        };
    public:
        void RegisterRoute(const String& pattern, engine::MetaTypeSharedPtr metatype, MethodType method = All);

        api::ApiCommandBaseSharedPtr Match(const String& uri, MethodType method, std::map<String, String>& path, std::map<String, String>& queries);
    private:
        std::vector<RouteItem> Routes;
    };
    DECLARE_SHARED_PTR(Route)
}