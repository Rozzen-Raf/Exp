#pragma once
#include "Types.h"
#include "Worker.h"
struct Awaitable
{
	Awaitable(WorkerBaseSharedPtr worker, AwaitableData&& data_) : Worker(worker), data(data_)
	{

	}

	bool await_ready() noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
		data.continuation = caller;
		Worker->RegAwaitable(&data);
		return caller;
	}
	void await_resume() {}
private:
	AwaitableData data;
	WorkerBaseSharedPtr Worker;
};

