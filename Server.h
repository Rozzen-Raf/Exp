#pragma once
#include "ChainableTask.h"
#include "JsonParser.h"
#include "Listener.h"
#include "Sheduler.h"
#include "Session.h"
class Server
{
public:
    virtual ~Server() {}

    virtual CoroTaskVoid AsyncServerRun() = 0;

    virtual void SetArgs(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const JsonParser& json_data) = 0;

protected:

};
DECLARE_SHARED_PTR(Server)