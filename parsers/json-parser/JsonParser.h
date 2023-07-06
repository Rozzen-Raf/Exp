#pragma once

#include "Types.h"
#include "ParserUtils.h"
namespace api{
DECLARE_SHARED_PTR(ApiCommandBase)
}

namespace parse
{
class JsonParser
{
public:

    using BlockType = json;

    JsonParser() = default;
    explicit JsonParser(json&& data);
    bool ParseFromFile(StringView file_name);
    template<typename buffer_type>
    bool Parse(buffer_type& buffer)
    {
        if(buffer.empty())
            return false;

        try
        {
            Data = json::parse(buffer);

            return true;
        }
        catch(const std::exception& e)
        {
            ERROR(Parse, e.what());
            return false;
        }
    }

    template<typename buffer_type>
    static std::optional<ParseData> ParseToData(buffer_type buffer)
    {
        if(buffer.empty())
            return {};

        try
        {
            ParseData data;
            data.SetData(json::parse(buffer));
            return data;
        }
        catch(const std::exception& e)
        {
            ERROR(Parse, e.what());
            return {};
        }
    }

    template<class T>
    std::optional<T> GetValue(const String& key) const
    {
        try {
			auto value = Data.at(key);

			return value.get<T>();
		}
		catch (const std::exception& e)
		{
            ERROR(GetValue, e.what());
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

    void Reset();

    api::ApiCommandBaseSharedPtr ParseApiCommand(buffer_view& buffer);
private:
    json Data;
};
}