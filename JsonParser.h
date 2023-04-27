#pragma once
#include "json.hpp"
#include "Types.h"
using json = nlohmann::json;

class JsonParser
{
    void Parse(std::span<std::byte> data);

    template<class T>
    std::optional<T> GetValue(const String& key)
    {
        try {
			auto value = Data.at(key);

			return value.get<T>();
		}
		catch (const std::exception& e)
		{
            std::cerr << e.what() << '\n';
            return {};
        }
    }
private:
    json Data;
};