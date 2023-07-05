#pragma once

#include "MySQLTypes.hpp"

namespace mysql
{
    //bool PrepareStmtFromType(PreparedStatementUniquePtr& stmt, int index, const std::any& value, DataType type);

    template<typename T>
    DataType GetType()
    {
        if constexpr (std::is_same_v<int8_t, T>)
        {
            return DataType::CHAR;
        }
        else if constexpr(std::is_same_v<int16_t, T>)
        {
            return DataType::SHORT;
        }
        else if constexpr(std::is_same_v<int32_t, T>)
        {
            return DataType::INTEGER;
        }
        else if constexpr(std::is_same_v<int64_t, T>)
        {
            return DataType::LONG;
        }
        else if constexpr(std::is_same_v<uint64_t, T>)
        {
            return DataType::ULONG;
        }
        else if constexpr(std::is_same_v<String, T>)
        {
            return DataType::STRING;
        }
        else if constexpr(std::is_same_v<std::chrono::system_clock::time_point, T>)
        {
            return DataType::TIMESTAMP;
        }
        else 
        {
            static_assert(!sizeof(T), "Unknown DataType");
        }
    }
}