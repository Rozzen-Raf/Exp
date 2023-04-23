#pragma once
#include "Types.h"
#include "Worker.h"
struct Awaitable
{
    Awaitable(WorkerBaseSharedPtr worker, AwaitableData&& data_, std::coroutine_handle<> handle) :data(data_), Worker(worker),  handle_(handle)
	{

	}

	~Awaitable() = default;

	bool await_ready() noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
		data.continuation = caller;
		Worker->RegAwaitable(&data);
		return handle_;
	}
    AwaitableResult await_resume() const
	{
		return data.result;
	}
private:
	AwaitableData data;
	WorkerBaseSharedPtr Worker;
	std::coroutine_handle<> handle_;
};

