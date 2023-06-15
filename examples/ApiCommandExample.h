#pragma once
#include "ApiCommandBase.h"

namespace api
{

template<class Tools>
class Print : public ApiCommandWithTools<Tools>
{
    METATYPE
public:    
    
    virtual task::CoroTask<String> ExecutionCommand() final;
};

template<class Tools>
task::CoroTask<String> Print<Tools>::ExecutionCommand()
{
    auto print_data = this->mTools.mParser.template GetValue<String>("PrintData");
    auto message_id = this->mTools.mParser.template GetValue<ID_t>("MessageID");
    auto res = std::make_pair(-1, Result::Failed);
    if(print_data.has_value() && message_id.has_value())
    {
        res = std::make_pair(message_id.value(), Result::Success);
    }

    typename Tools::ParserType parser;
    parser.SetValue("MessageID", res.first);
    parser.SetValue("ResultID", res.second);

    co_return parser.Dump();
}
}