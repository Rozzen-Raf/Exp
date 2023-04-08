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
		//unreg();
	}

	void unreg()
	{
		//Worker->UnregAwaitable(&data);
	}

	bool await_ready() noexcept { return false; }
	void await_suspend(std::coroutine_handle<> caller) {
		data.continuation = caller;
		Worker->RegAwaitable(&data);
		return ;
	}
	AwaitableResult await_resume() 
	{
		return data.result;
	}
private:
	AwaitableData data;
	WorkerBaseSharedPtr Worker;
};

