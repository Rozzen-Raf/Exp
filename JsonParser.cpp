#include "JsonParser.h"

void JsonParser::Parse(std::span<std::byte> data)
{
    try
    {
        Data = json::parse(data);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}