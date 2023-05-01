#include "JsonParser.h"
//-----------------------------------------------------------

JsonParser::JsonParser(json&& data) : Data(data)
{}
//-----------------------------------------------------------
void JsonParser::Parse(buffer& data)
{
    try
    {
        Data = json::parse(data);
    }
    catch(const std::exception& e)
    {
        ERROR(JsonParser, e.what());
    }
}
//-----------------------------------------------------------

void JsonParser::ParseFromFile(StringView file_name)
{
    try
    {
        std::ifstream f(file_name.data());

        Data = json::parse(f);
    }
    catch(const std::exception& e)
    {
        ERROR(JsonParser, e.what());
    }
}
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------