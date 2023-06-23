#include "HttpUtils.h"

namespace http
{
    MethodType MethodFromString(StringView str)
    {
        MethodType res = Unknown;
        if(str == "GET")
        {
            res = GET;
        }
        else if(str == "POST")
        {
            res = POST;
        }
        else if(str == "PUT")
        {
            res = PUT;
        }
        else if(str == "DELETE")
        {
            res = DELETE;
        }
        else if(str == "PATCH")
        {
            res = PATCH;
        }

        return res;
    }

    String MethodToString(MethodType method)
    {
        String res = "UNKNOWN";

        switch(method)
        {
            case GET:
                res = "GET";
                break;
            case POST:
                res = "POST";
                break;
            case PUT:
                res = "PUT";
                break;
            case DELETE:
                res = "DELETE";
                break;
            case PATCH:
                res = "PATCH";
                break;
            default:
                break;
        }

        return res;
    }
}