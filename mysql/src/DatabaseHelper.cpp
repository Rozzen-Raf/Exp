#include "DatabaseHelper.hpp"
namespace mysql
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
        return true;
    }
    //-----------------------------------------------------------

    void Database::Close()
    {
    }
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    //-----------------------------------------------------------
}