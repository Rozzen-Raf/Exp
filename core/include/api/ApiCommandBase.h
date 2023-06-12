#pragma once
#include "MetaType.h"

namespace api
{
enum class Result
{
	Failed, Success, UnknownCommand
};

class ApiCommandBase
{
public:
    virtual ~ApiCommandBase() {}

    virtual String ExecutionCommand() = 0;
};

template<class Parser>
class ApiCommandWithTools : public ApiCommandBase
{
public:
    virtual ~ApiCommandWithTools(){}

    inline void SetParser(Parser&& parser)
    {
        mParser = std::move(parser);
    } 
protected:
    Parser mParser;
};
DECLARE_SHARED_PTR(ApiCommandBase)
}
