#include "JsonUtils.h"

std::pair<ApiCommandBaseSharedPtr, JsonParser> ParseJsonApiCommand(buffer_view& buffer)
{
    if(!buffer.size())
        return {nullptr, {}};

    try
    {
        JsonParser parser;
        parser.Parse(buffer);

        auto api_command = parser.GetValue<String>("Command");

        if(!api_command.has_value() || api_command.value().empty())
             return {nullptr, {}};

        String& value = api_command.value();
        ApiCommandBaseSharedPtr api_command_execute = std::static_pointer_cast<ApiCommandBase>(MetaData::GetMetaData()->Create(value));

        return {api_command_execute, std::move(parser)};
    }
    catch(const std::exception& e)
    {
        ERROR(JsonParser, e.what());
        return {nullptr, {}};
    }
}
