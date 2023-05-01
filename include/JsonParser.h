#pragma once
#include "json.hpp"
#include "Types.h"
using json = nlohmann::json;

class JsonParser
{
public:
    JsonParser() = default;
    explicit JsonParser(json&& data);
    //JsonParser(JsonParser&& )
    void Parse(buffer& data);
    void ParseFromFile(StringView file_name);

    template<class T>
    std::optional<T> GetValue(const String& key) const
    {
        try {
			auto value = Data.at(key);

			return value.get<T>();
		}
		catch (const std::exception& e)
		{
            ERROR(JsonParser, e.what());
            return {};
        }
    }
private:
    json Data;
};
