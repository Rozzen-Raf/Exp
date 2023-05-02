#pragma once
#include "Socket.h"
#include "RegisterMediator.h"
#include "ChainableTask.h"
#include "Sheduler.h"
class Listener : public Socket
{
public:
    explicit Listener(RegisterMediatorBasePtr reg) noexcept: Register(reg){}
    CoroTask<AwaitableResult> AsyncAccept(ShedulerSharedPtr shared) noexcept;

    inline void SetRegister(RegisterMediatorBasePtr reg) noexcept { Register = reg;}

private:
    RegisterMediatorBasePtr Register;
};
