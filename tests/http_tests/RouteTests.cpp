#include "Headers.h"
#include "Route.h"

TEST_CASE("RouteTest", "RouteTest")
{
    http::Route r;

    r.RegisterRoute("/print", engine::MetaData::GetMetaData()->GetMetaType("Print"));

    std::map<String, String> path, queries;
    auto api_command = r.Match("/print", http::GET, path, queries);
    REQUIRE(api_command != nullptr);

    r.RegisterRoute("/print/{id}", engine::MetaData::GetMetaData()->GetMetaType("Print"));
    api_command = r.Match("/print/3", http::GET, path, queries);
    REQUIRE(api_command != nullptr);

    path.clear();
    queries.clear();

    api_command = r.Match("/print/3?user={user-name}", http::GET, path, queries);
    REQUIRE(api_command != nullptr);
}