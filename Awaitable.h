#pragma once
#include "Types.h"
#include "Worker.h"
struct Awaitable
{
	Awaitable(WorkerBaseSharedPtr worker, AwaitableData&& data_) : Worker(worker), data(data_)
	{

	}

	~Awaitable()
	{
		unreg();
	}

	void unreg()
	{
		Worker->UnregAwaitable(&data);
	}

	bool await_ready() noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
		data.continuation = caller;
		Worker->RegAwaitable(&data);
		return caller;
	}
	AwaitableResult await_resume() 
	{
		return data.result;
	}
private:
	AwaitableData data;
	WorkerBaseSharedPtr Worker;
};

