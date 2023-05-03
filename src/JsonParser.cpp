#include "JsonParser.h"
//-----------------------------------------------------------

JsonParser::JsonParser(json&& data) : Data(data)
{}
//-----------------------------------------------------------
bool JsonParser::Parse(buffer_view_const& data)
{
    try
    {
        Data = json::parse(data);
        return true;
    }
    catch(const std::exception& e)
    {
        ERROR(JsonParser, e.what());
        return false;
    }
}
//-----------------------------------------------------------

bool JsonParser::Parse(buffer& data)
{
    if(data.empty())
        return false;

    try
    {
        Data = json::parse(data);
        return true;
    }
    catch(const std::exception& e)
    {
        ERROR(JsonParser, e.what());
        return false;
    }
}
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
