#include "DatabaseHelper.hpp"
namespace mariadb
{
    //-----------------------------------------------------------

    Database::Database(const io::IPEndPoint& endpoint, const AuthSettings& auth)
    {
        Connect(endpoint, auth);
    }
    //-----------------------------------------------------------

    Database::~Database()
    {
        Close();
    }
    //-----------------------------------------------------------

    bool Database::Connect(const io::IPEndPoint& endpoint, const AuthSettings& auth)
    {
        try
        {
            sql::Driver* driver = sql::mariadb::get_driver_instance();

            String path = "jdbc:mariadb://";
            path.insert(path.size(), endpoint.GetIP());
            path.push_back(':');
            path.insert(path.size(), std::to_string(endpoint.GetPort()));
            path.append("//");
            path.insert(path.size(), auth.database);
            
            sql::SQLString url(path);

            sql::Properties properties({{"user", auth.login}, {"password", auth.password}});
            ConnPtr.reset(driver->connect(url, properties));

            if(!ConnPtr)
            {
                ERROR(Database, "Error connection to mariadb");
                return false;
            }

            return true;
        }
        catch(sql::SQLException& e)
        {
            std::stringstream stream;
            stream << "Error connection to mariadb: " << e.what() << ". Error code: " << e.getErrorCode();
            ERROR(Database, stream.str());
            return false;
        }
    }
    //-----------------------------------------------------------

    ResultSetSharedPtr Database::ExecuteQuery(const String& query)
    {
        try
        {
            PreparedStatementUniquePtr stmnt(ConnPtr->prepareStatement(query));

            return ResultSetSharedPtr(stmnt->executeQuery());
        }
        catch(SQLException& e)
        {
            std::stringstream stream;
            stream << "Error connection to mariadb: " << e.what() << ". Error code: " << e.getErrorCode();
            ERROR(Database, stream.str());
            return nullptr;
        }
    }
    //-----------------------------------------------------------

    void Database::Close()
    {
        if(ConnPtr)
            ConnPtr->close();
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    //-----------------------------------------------------------
}