#pragma once

#include "conncpp.hpp"
#include "Types.h"

namespace mariadb
{
    using namespace sql;

    struct AuthSettings
    {
        String database;
        String login;
        String password;
    };

    enum class MariaDataType
    {
          ARRAY = 1
        , STRING
        , BOOLEAN
        , CHAR
        , DOUBLE
        , INTEGER
        , FLOAT
        , SHORT
        , LONG
        , ULONG
        , TIMESTAMP
    };
    
    struct BindingItem
    {
        MariaDataType type;
        std::any value;
    };
    
    template<typename T>
    MariaDataType GetType();

    class PreparedStatementBindingData
    {
        template<typename T>
        inline void Push(const T& value)
        {
            BindingData.emplace_back(GetType<T>(), value);
        }

        template<typename T>
        inline void Push(T&& value)
        {
            BindingData.emplace_back(GetType<T>(), std::move(value));
        }

        private:
        std::vector<BindingItem> BindingData;
    };

    UNIQUE_PTR(ResultSet)
    UNIQUE_PTR(Connection)
    UNIQUE_PTR(PreparedStatement)

    SHARED_PTR(ResultSet)
}