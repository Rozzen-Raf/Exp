#include "MySQLUtils.hpp"

namespace mysql
{
    // bool PrepareStmtFromType(PreparedStatementUniquePtr& stmt, int index, const BindingItem& item)
    // {
    //     if(!stmt)
    //     {
    //         ERROR(PrepareStmt, "Statement is not exist");
    //         return false;
    //     }

    //     auto& value = item.value;

    //     try
    //     {
    //         switch(item.type)
    //         {
    //             case DataType::STRING:
    //                 stmt->setString(index, std::any_cast<String>(value));
    //                 break;
    //             case DataType::BOOLEAN:
    //                 stmt->setBoolean(index, std::any_cast<bool>(value));
    //                 break;
    //             case DataType::CHAR:
    //                 stmt->setByte(index, std::any_cast<char>(value));
    //                 break;
    //             case DataType::DOUBLE:
    //                 stmt->setDouble(index, std::any_cast<double>(value));
    //                 break;
    //             case DataType::INTEGER:
    //                 stmt->setInt(index, std::any_cast<int>(value));
    //                 break;
    //             case DataType::FLOAT:
    //                 stmt->setInt(index, std::any_cast<float>(value));
    //                 break;
    //             case DataType::SHORT:
    //                 stmt->setShort(index, std::any_cast<int16_t>(value));
    //                 break;
    //             case DataType::LONG:
    //                 stmt->setLong(index, std::any_cast<int64_t>(value));
    //                 break;
    //             case DataType::ULONG:
    //                 stmt->setUInt64(index, std::any_cast<uint64_t>(value));
    //                 break;
    //             case DataType::TIMESTAMP:
    //                 auto msec = std::chrono::microseconds(std::any_cast<std::chrono::system_clock::time_point>(value).time_since_epoch().count());
    //                 stmt->setUInt64(index, msec.count());
    //                 break;
    //         }
    //         return true;
    //     }
    //     catch(const std::exception& e)
    //     {
    //         ERROR(PrepareStmt, e.what());
    //         return false;
    //     } 
    // }
}