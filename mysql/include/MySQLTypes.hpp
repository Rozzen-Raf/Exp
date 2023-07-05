#pragma once

#include "Types.h"
#include "mysql/mysql.h"

namespace mysql
{
    struct AuthSettings
    {
        String database;
        String login;
        String password;
    };

    enum class DataType
    {
          STRING = 1
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
        DataType type;
        std::any value;
    };
    
    template<typename T>
    DataType GetType();

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
}