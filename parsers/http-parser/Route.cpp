#include "Route.h"
//------------------------------------------------------------

void Route::RegisterRoute(const String& pattern, engine::MetaTypeSharedPtr metatype, MethodType method)
{
    Routes.emplace_back(pattern, metatype, method);
}
//------------------------------------------------------------

api::ApiCommandBaseSharedPtr Route::Match(const String& uri, MethodType method, std::map<String, String>& path, std::map<String, String>& queries)
{
    if(Routes.empty())
        return nullptr;

    return nullptr;
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------

