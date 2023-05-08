#include "JsonUtils.h"

std::pair<ApiCommandBaseSharedPtr, JsonParser> ParseJsonApiCommand(buffer_view buffer)
{
    if(!buffer.size())
        return {nullptr, {}};

    try
    {
        JsonParser parser;
        parser.Parse(buffer);

        auto command = parser.GetValue<json>("ApiCommand");

        if(!command.has_value())
            return {nullptr, {}};

        auto api_command = command.value().at("Command");

        if(api_command.is_null() || !api_command.is_string())
             return {nullptr, {}};

        auto value = api_command.get<String>();
        ApiCommandBaseSharedPtr api_command_execute = std::static_pointer_cast<ApiCommandBase>(MetaData::GetMetaData()->Create(value));

        return {api_command_execute, JsonParser{std::move(command.value())}};
    }
    catch(const std::exception& e)
    {
        ERROR(JsonParser, e.what());
        return {nullptr, {}};
    }
}
