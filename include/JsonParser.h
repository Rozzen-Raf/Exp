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
    bool Parse(buffer_view_const& data);
    bool ParseFromFile(StringView file_name);
    bool Parse(buffer_ptr data);

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

    template<typename T>
	inline void SetValue(const String& key_name, const T& value)
	{
		Data[key_name] = value;
	}

	template<typename T>
	inline void SetValue(String&& key_name, T&& value)
	{
		Data[std::move(key_name)] = std::move(value);
	}

    inline String Dump()
    {
        return Data.dump();
    }
private:
    json Data;
};
