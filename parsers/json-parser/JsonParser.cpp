#include "JsonParser.h"
#include "MetaType.h"
namespace parse{
//-----------------------------------------------------------

JsonParser::JsonParser(json&& data) : Data(data)
{}
//-----------------------------------------------------------

bool JsonParser::ParseFromFile(StringView file_name)
{
    try
    {
        std::ifstream f(file_name.data());

        if(!f.is_open())
        {
            ERROR(JsonParser, "Failed to open file");
            return false;
        }

        Data = json::parse(f);
        return true;
    }
    catch(const std::exception& e)
    {
        ERROR(ParseFromFile, e.what());
        return false;
    }
}
//-----------------------------------------------------------

api::ApiCommandBaseSharedPtr JsonParser::ParseApiCommand(buffer_view& buffer)
{
    if(!buffer.size())
        return nullptr;

    try
    {
        Parse(buffer);

        auto api_command = GetValue<String>("Command");

        if(!api_command.has_value() || api_command.value().empty())
             return nullptr;

        String& value = api_command.value();
        api::ApiCommandBaseSharedPtr api_command_execute = std::static_pointer_cast<api::ApiCommandBase>(engine::MetaData::GetMetaData()->Create(value));

        return api_command_execute;
    }
    catch(const std::exception& e)
    {
        ERROR(ParseApiCommand, String(e.what()) + " " + String((char*)(buffer.data()), buffer.size()));
        return nullptr;
    }
}
//-----------------------------------------------------------

void JsonParser::Reset()
{
    Data.clear();
}
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
}
