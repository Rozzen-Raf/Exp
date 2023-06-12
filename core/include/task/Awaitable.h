#pragma once
#include "Types.h"
#include "Worker.h"
namespace task
{
struct Awaitable
{
    Awaitable(engine::WorkerBaseSharedPtr worker, engine::AwaitableData&& data_, std::coroutine_handle<> handle) :data(data_), Worker(worker),  handle_(handle)
        {

        }

        ~Awaitable() = default;

        void Unreg()
        {
                Worker->UnregAwaitable(&data);
        }

        void SetUnregMode(bool mode)
        {
                data.NeedUnreg = mode;
        }

        bool await_ready() noexcept { return false; }
    void await_suspend(std::coroutine_handle<> caller) {
                data.continuation = caller;
                Worker->RegAwaitable(&data);
        return;
        }
        engine::AwaitableResult await_resume() const
        {
                return data.result;
        }

        template <typename U>
        U&& await_transform(U&& awaitable) noexcept
        {
                return static_cast<U&&>(awaitable);
        }
private:
        engine::AwaitableData data;
        engine::WorkerBaseSharedPtr Worker;
        std::coroutine_handle<> handle_;
};
}