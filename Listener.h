#pragma once
#include "Socket.h"
#include "RegisterMediator.h"
#include "ChainableTask.h"
#include "Sheduler.h"
class Listener : public Socket
{
public:
    explicit Listener(RegisterMediatorBasePtr reg) noexcept: Register(reg){}
    CoroTask<Socket> AsyncAccept(ShedulerSharedPtr shared) noexcept;

private:
    RegisterMediatorBasePtr Register;
};