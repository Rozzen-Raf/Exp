#include "MariaDBUtils.hpp"

namespace mariadb
{
    bool PrepareStmtFromType(PreparedStatementUniquePtr& stmt, int index, const std::any& value, MariaDataType type)
    {
        if(!stmt)
        {
            ERROR(PrepareStmt, "Statement is not exist");
            return false;
        }

        try
        {
            switch(type)
            {
                case MariaDataType::ARRAY:
                    stmt->setBytes(index, std::any_cast<bytes*>(value));
                    break;
                case MariaDataType::STRING:
                    stmt->setString(index, std::any_cast<String>(value));
                    break;
                case MariaDataType::BOOLEAN:
                    stmt->setBoolean(index, std::any_cast<bool>(value));
                    break;
                case MariaDataType::CHAR:
                    stmt->setByte(index, std::any_cast<char>(value));
                    break;
                case MariaDataType::DOUBLE:
                    stmt->setDouble(index, std::any_cast<double>(value));
                    break;
                case MariaDataType::INTEGER:
                    stmt->setInt(index, std::any_cast<int>(value));
                    break;
                case MariaDataType::FLOAT:
                    stmt->setInt(index, std::any_cast<float>(value));
                    break;
                //case MariaDataType::
            }
            return true;
        }
        catch(const std::exception& e)
        {
            ERROR(PrepareStmt, e.what());
            return false;
        } 
    }
}