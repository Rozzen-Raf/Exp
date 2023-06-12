#pragma once
#include "Socket.h"
#include "RegisterMediator.h"
#include "ChainableTask.h"
#include "Sheduler.h"
namespace io{
class Listener : public Socket
{
public:
    explicit Listener(engine::RegisterMediatorBasePtr reg) noexcept: Register(reg){}
    task::CoroTask<engine::AwaitableResult> AsyncAccept(task::ShedulerSharedPtr shared, engine::WorkerType type) noexcept;
    engine::AwaitableResult Accept() noexcept;

    inline void SetRegister(engine::RegisterMediatorBasePtr reg) noexcept { Register = reg;}

private:
    engine::RegisterMediatorBasePtr Register;
};
}
