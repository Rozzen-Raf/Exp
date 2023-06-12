#pragma once
#include "ApiCommandBase.h"

namespace api
{
template<class Parser>
class Print : public ApiCommandWithTools<Parser>
{
    METATYPE
public:
    virtual String ExecutionCommand() final;
};

template<class Parser>
String Print<Parser>::ExecutionCommand()
{
    auto print_data = this->mParser.template GetValue<String>("PrintData");
    auto message_id = this->mParser.template GetValue<ID_t>("MessageID");
    auto res = std::make_pair(-1, Result::Failed);
    if(print_data.has_value() && message_id.has_value())
    {
        res = std::make_pair(message_id.value(), Result::Success);
    }

    Parser parser;
    parser.SetValue("MessageID", res.first);
    parser.SetValue("ResultID", res.second);

    return parser.Dump();
}
}