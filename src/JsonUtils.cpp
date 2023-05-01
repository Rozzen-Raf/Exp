#include "JsonUtils.h"

std::pair<ApiCommandBaseSharedPtr, json> ParseJsonApiCommand(buffer_ptr buffer)
{
    if(!buffer)
        return {nullptr, {}};

    try
    {
        json data = json::parse(*buffer.get());

        auto command = data.at("ApiCommand");

        if(command.is_null())
            return {nullptr, {}};

        auto api_command = command.at("Command");

        if(api_command.is_null() || !api_command.is_string())
             return {nullptr, {}};

        auto value = api_command.get<String>();
        ApiCommandBaseSharedPtr api_command_execute = std::static_pointer_cast<ApiCommandBase>(MetaData::GetMetaData()->Create(value));

        return {api_command_execute, std::move(command)};
    }
    catch(const std::exception& e)
    {
        ERROR(JsonParser, e.what());
        return {nullptr, {}};
    }
}