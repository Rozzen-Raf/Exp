#pragma once

#include "MariadDBTypes.hpp"

namespace mariadb
{
    bool PrepareStmtFromType(PreparedStatementUniquePtr& stmt, int index, const std::any& value, MariaDataType type);

    template<typename T>
    MariaDataType GetType()
    {
        if constexpr (std::is_same_v<int8_t, T>)
        {
            return MariaDataType::CHAR;
        }
        else if constexpr(std::is_same_v<int16_t, T>)
        {
            return MariaDataType::SHORT;
        }
        else if constexpr(std::is_same_v<int32_t, T>)
        {
            return MariaDataType::INTEGER;
        }
        else if constexpr(std::is_same_v<int64_t, T>)
        {
            return MariaDataType::LONG;
        }
        else if constexpr(std::is_same_v<uint64_t, T>)
        {
            return MariaDataType::ULONG;
        }
        else if constexpr(std::is_same_v<String, T>)
        {
            return MariaDataType::STRING;
        }
        else if constexpr(std::is_same_v<std::chrono::system_clock::time_point, T>)
        {
            return MariaDataType::TIMESTAMP;
        }
        else 
        {
            static_assert(!sizeof(T), "Unknown MariaDataType");
        }
    }
}