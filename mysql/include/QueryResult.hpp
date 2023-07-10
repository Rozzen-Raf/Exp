#pragma once

#include "MySQLTypes.hpp"

namespace mysql
{
    class QueryResultItem
    {
    public:
        QueryResultItem() = default;
    private:
        std::vector<String> row;
    };

    class QueryResult
    {
    public:
        QueryResult() = default;
    private:
        std::vector<QueryResultItem> rows;
    };
}