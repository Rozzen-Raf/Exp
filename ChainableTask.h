#pragma once
#include "Types.h"
#include "FinalContinuation.h"
struct CoroTaskVoid
{
	struct CoroTaskPromise
	{
		using handle_t = std::coroutine_handle<CoroTaskPromise>;
		CoroTaskVoid get_return_object()
		{
			return CoroTaskVoid{ handle_t::from_promise(*this) };
		}

		std::suspend_always initial_suspend() noexcept { return {}; }
		AwaitableFinalContinuation final_suspend() noexcept {
			return {}; 
		}
		void unhandled_exception() { std::terminate(); }
		void return_void() {}

		~CoroTaskPromise()
		{
		}

		void set_done_callback(func_t callback) noexcept { done_callback = callback; }

	private:
		friend struct CoroTaskVoid;
		friend struct AwaitableFinalContinuation;
		std::coroutine_handle<> continuation = std::noop_coroutine();
		std::coroutine_handle<> handle = std::noop_coroutine();
		func_t done_callback;
	};

	using promise_type = CoroTaskPromise;
	explicit CoroTaskVoid(promise_type::handle_t handle) : handle_(handle) 
	{
		handle.promise().handle = handle_;
	}
	CoroTaskVoid(const CoroTaskVoid&) = delete;
	CoroTaskVoid(CoroTaskVoid&& task)
		: handle_(std::exchange(task.handle_, nullptr)) {}
	CoroTaskVoid& operator=(const CoroTaskVoid&) = delete;
	CoroTaskVoid& operator=(CoroTaskVoid&& task) {
		handle_ = std::exchange(task.handle_, nullptr);
		return *this;
	}
	
	~CoroTaskVoid()
	{
		if (handle_)
			handle_.destroy();
	}
	bool await_ready() noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
		handle_.promise().continuation = caller;
		return handle_;
	}
	void await_resume() {}

private:
	friend class Task;
	friend class Sheduler;
	func_t GetFunc()const noexcept { return [&]() {if (handle_) handle_.resume(); }; }

	void SetDoneCallback(func_t callback)
	{
		if (handle_)
			handle_.promise().set_done_callback(callback);
	}

	const promise_type::handle_t& GetHandle()const  noexcept { return handle_; }
private:
	promise_type::handle_t handle_;
};