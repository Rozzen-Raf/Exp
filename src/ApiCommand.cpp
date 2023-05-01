#include "ApiCommand.h"

METATYPE_DEF(Print)
//-------------------------------------------------------------

std::pair<ID_t, Result> Print::ExecutionCommand(const JsonParser& json_data)
{
    auto print_data = json_data.GetValue<String>("PrintData");
    auto message_id = json_data.GetValue<ID_t>("MessageID");

    if(!print_data.has_value() || !message_id.has_value())
        return {-1, Failed};

    LOG(Print, print_data.value());

    return std::make_pair(message_id.value(), Success);
}
//-------------------------------------------------------------
