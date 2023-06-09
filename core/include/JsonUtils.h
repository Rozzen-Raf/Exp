#pragma once
#include "ApiCommandBase.h"

template<class Parser>
std::pair<ApiCommandBaseSharedPtr, Parser> ParseApiCommand(buffer_view& buffer)
{
    if(!buffer.size())
        return {nullptr, {}};

    try
    {
        Parser parser;
        parser.Parse(buffer);

        auto api_command = parser.template GetValue<String>("Command");

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
