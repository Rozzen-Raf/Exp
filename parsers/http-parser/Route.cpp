#include "Route.h"
//------------------------------------------------------------

void Route::RegisterRoute(const String& pattern, engine::MetaTypeSharedPtr metatype, MethodType method)
{
    Routes.emplace_back(pattern, metatype, method);
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------

