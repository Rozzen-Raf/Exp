#include "Utils.h"
namespace utils
{
    std::vector<String> split(const String& str, const String& delimiter)
    {
        std::vector<String> res;
        size_t pos = 0;
        size_t last = 0; size_t next = 0; 
        while ((next = str.find(delimiter, last)) != String::npos) 
        {  
            auto token = str.substr(last, next-last);
            if(!token.empty())
                res.emplace_back(token);  
            last = next + 1; 
        }

        res.emplace_back(str.substr(last));

        return res;
    }
}