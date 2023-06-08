#pragma once
#include "MetaType.h"
#include "JsonParser.h"

enum class Result
{
	Failed, Success, UnknownCommand
};

class ApiCommandBase
{
public:
    virtual ~ApiCommandBase() {}

    virtual std::pair<ID_t, Result> ExecutionCommand(const JsonParser& buffer) = 0;
};
DECLARE_SHARED_PTR(ApiCommandBase)

class Print : public ApiCommandBase
{
    METATYPE
public:
    virtual std::pair<ID_t, Result> ExecutionCommand(const JsonParser& buffer) final;
};


