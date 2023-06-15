#pragma once
#include "MetaType.h"
#include "ChainableTask.h"
namespace api
{
enum class Result
{
	Failed, Success, UnknownCommand
};

template<class Tools>
class ApiCommandWithTools;

class ApiCommandBase
{
public:
    virtual ~ApiCommandBase() {}

    virtual task::CoroTask<String> ExecutionCommand() = 0;

    template<class Tools>
    void SetTools(Tools&& tools)
    {
        if(ApiCommandWithTools<Tools>* p = dynamic_cast<ApiCommandWithTools<Tools>*>(this))
        {
            p->SetTools(std::move(tools));
        }
    }
};

template<class Parser>
struct DefaultTools
{
    using ParserType = Parser;
    Parser mParser;
};

template<class Tools>
class ApiCommandWithTools : public ApiCommandBase
{
public:
    virtual ~ApiCommandWithTools(){}

    inline void SetTools(Tools&& tools)
    {
        mTools = std::move(tools);
    } 
protected:
    Tools mTools;
};
DECLARE_SHARED_PTR(ApiCommandBase)
}
