#pragma once
#include "Types.h"
#include "Control.h"
struct Awaitable
{
	Awaitable(ControlBaseSharedPtr control, AwaitableData&& data_) : Control(control), data(data_)
	{

	}

	bool await_ready() noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
		data.continuation = caller;
		Control->RegAwaitable(&data);
		return caller;
	}
	void await_resume() {}
private:
	AwaitableData data;
	ControlBaseSharedPtr Control;
};

