#pragma once

#include "Types.h"
#include "json.hpp"

namespace parse
{
    using json = nlohmann::json;
    class BadParse : public std::logic_error
    {
        public:
            BadParse() : std::logic_error("Bad parse")
            {}
    };

    class DataItem
    {
        public:
        DataItem() = default;

        template<typename T>
        DataItem(T&& value) : ItemData(std::move(value))
        {}

        template<typename T>
        DataItem(const T& value) : ItemData(value)
        {}

        template<typename T>
        inline T get() const
        {
            return std::any_cast<T>(ItemData);
        }
        private:
        std::any ItemData;
    };

    class Data
    {
        public:
        template<typename T>
        inline void set(const String& key, const T& value)
        {
            mData.emplace(key, value);
        }

        template<typename T>
        inline void set(String&& key, T&& value)
        {
            mData.emplace(std::forward<String>(key), std::forward<T>(value));
        }
        
        template<typename T>
        inline T get(const String& key) const
        {
            auto find_iter = mData.find(key);
            if(find_iter == mData.end())
                throw BadParse();
            return find_iter->second.get<T>();
        }

        inline DataItem& operator[](const String& key) 
        {
            return mData[key];
        }

        private:
        friend class ParseData;

        const DataItem& at(const String& key)
        {
            auto find_iter = mData.find(key);
            if(find_iter == mData.end())
                throw BadParse();
            return find_iter->second;
        }

        std::map<String, DataItem> mData;
    };

    class ParseData
    {
        public:
        ParseData() = default;
        ParseData(ParseData&& data) : mData(std::move(data.mData))
        {}

        ParseData(const ParseData& data) : mData(data.mData)
        {}

        template<typename T>
        inline void SetData(const T& data) noexcept
        {
            mData = data;
        }

        template<typename T>
        inline void SetData(T&& data) noexcept
        {
            mData = std::move(data);
        }

        template<typename T>
        inline void SetValue(const String& key, const T& value)
        {
            auto visitor = [&key, &value](auto data)
            {
                data[key] = value;
            };

            std::visit(visitor, mData);
        }

        template<typename T>
        inline void SetValue(String&& key, T&& value)
        {
            auto visitor = [key = std::move(key), value = std::move(value)](auto data)
            {
                data[std::move(key)] = std::move(value);
            };

            std::visit(visitor, mData);
        }

        template<typename T>
        inline std::optional<T> GetValue(const String& key)
        {
            auto visitor = [&key](auto data) -> std::optional<T>
            {
                try
                {
                    auto data_item = data.at(key);
                    return data_item.template get<T>();
                }
                catch(const std::exception& e)
                {
                    LOG(ParseData, e.what());
                    return {};
                }
            };

            return std::visit(visitor, mData);
        }
        private:
        std::variant<json, Data> mData;
    };
}