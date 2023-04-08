#pragma once
#include "Types.h"
#include "Worker.h"
struct Awaitable
{
	Awaitable(WorkerBaseSharedPtr worker, AwaitableData&& data_, std::coroutine_handle<> handle) : Worker(worker), data(data_), handle_(handle)
	{

	}

	~Awaitable()
	{
		//unreg();
	}

	void unreg()
	{
		//Worker->UnregAwaitable(&data);
	}

	bool await_ready() noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
		data.continuation = caller;
		Worker->RegAwaitable(&data);
		return handle_;
	}
	AwaitableResult await_resume() 
	{
		return data.result;
	}
private:
	AwaitableData data;
	WorkerBaseSharedPtr Worker;
	std::coroutine_handle<> handle_;
};

