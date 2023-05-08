#include "JsonParser.h"
//-----------------------------------------------------------

JsonParser::JsonParser(json&& data) : Data(data)
{}
//-----------------------------------------------------------

bool JsonParser::ParseFromFile(StringView file_name)
{
    try
    {
        std::ifstream f(file_name.data());

        Data = json::parse(f);
        return true;
    }
    catch(const std::exception& e)
    {
        ERROR(JsonParser, e.what());
        return false;
    }
}
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
