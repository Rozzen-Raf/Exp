#pragma once

#include "HttpRequestParser.h"
#include "ApiCommandBase.h"
#include "Route.h"

namespace http
{
    template<class BodyParser>
    class HttpParser
    {
        public:
            api::ApiCommandBaseSharedPtr ParseApiCommand(buffer_view& buffer);

            void SetRoute(RouteSharedPtr route) noexcept;

            void Reset();

            template<class T>
            std::optional<T> GetValue(const String& key) const
            {
                return mParser.template GetValue<T>(key);
            }
        private:
            RouteSharedPtr mRoute;
            RequestImpl mRequest;
            std::map<String, String> RequestInPath;
            std::map<String, String> Queries;
            BodyParser mParser;
    };

        template<class BodyParser>
    api::ApiCommandBaseSharedPtr HttpParser<BodyParser>::ParseApiCommand(buffer_view& buffer)
    {
        Reset();
        HttpRequestParser parser;

        auto res = parser.parse(mRequest, engine::BufferHelper(buffer));

        if(res.result != ParseCompleted)
        {
            ERROR(HttpParser, res.err_message);
            return nullptr;
        }

        mParser.Parse(mRequest.GetBody());

        api::ApiCommandBaseSharedPtr api_command = mRoute->Match(mRequest.GetUri(), mRequest.GetMethod(), RequestInPath, Queries);

        return api_command;
    }

    template<class BodyParser>
    void HttpParser<BodyParser>::SetRoute(RouteSharedPtr route) noexcept
    {
        mRoute = route;
    }

    template<class BodyParser>
    void HttpParser<BodyParser>::Reset()
    {
        mRequest.Reset();
        Queries.clear();
        RequestInPath.clear();
        mParser.Reset();
    }
}