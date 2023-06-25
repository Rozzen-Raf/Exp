#include "Route.h"
#include "Utils.h"
//------------------------------------------------------------
namespace http
{
void Route::RegisterRoute(const String& pattern, engine::MetaTypeSharedPtr metatype, MethodType method)
{
    auto split_pattern = utils::split(pattern, "/");

    Routes.emplace_back(std::move(split_pattern), metatype, method);
}
//------------------------------------------------------------

api::ApiCommandBaseSharedPtr Route::Match(const String& uri, MethodType method, std::map<String, String>& path, std::map<String, String>& queries)
{
    if(Routes.empty())
        return nullptr;

    auto query = utils::split(uri, "?");

    auto split_uri = utils::split(query[0], "/");

    auto find_route = Routes.end();

    for(auto route = Routes.begin(); route != Routes.end(); ++route)
    {
        bool no_match = false;
        if (route->Pattern.size() != split_uri.size())
            continue;

        for(int i = 0; i < split_uri.size(); ++i)
        {
            auto& pattern_token = route->Pattern[i];
            auto& uri_token = split_uri[i];
            if(pattern_token.size() > 2 && pattern_token.front() == '{' && pattern_token.back() == '}')
            {
                auto name = pattern_token.substr(1, pattern_token.size() - 2);
                path.emplace(name, uri_token);
            }
            else if(uri_token != pattern_token)
            {
                path.clear();
                no_match = true;
                break;
            }
        }

        if(no_match)
            continue;

        find_route = route;
        break;
    }

    api::ApiCommandBaseSharedPtr api_command;

    if(find_route == Routes.end())
        return api_command;

    api_command = std::static_pointer_cast<api::ApiCommandBase>(find_route->MetaType->Construct(nullptr));

    if(!api_command)
        return api_command;

    if(query.size() > 1)
    {
        auto split_query = utils::split(query[1], "&");
        for(auto query_token : split_query)
        {
            auto key_value = utils::split(query_token, "=");

            if(key_value.size() != 2)
                continue;
            
            queries.emplace(key_value[0], key_value[1]);
        }
    }

    return api_command;
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
}


