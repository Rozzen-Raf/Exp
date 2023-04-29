#pragma once
#include "ChainableTask.h"
#include "JsonParser.h"
#include "Listener.h"
#include "Sheduler.h"
class Server
{
public:
    virtual ~Server() {}

    virtual CoroTaskVoid AsyncServerRun() = 0;

    virtual void SetArgs(ShedulerSharedPtr sheduler, RegisterMediatorBasePtr reg, const JsonParser& json_data) = 0;

    virtual void CloseSession(const ID_t& id) noexcept = 0;

    //for test
    virtual void RedirectAll(const ID_t& id, buffer buf) noexcept = 0;

protected:

};
DECLARE_SHARED_PTR(Server)
DECLARE_WEAK_PTR(Server)
