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
		
		void unhandled_exception() 
		{
			auto exc = std::current_exception();
			std::rethrow_exception(exc);
		}
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

	void run() noexcept { if(handle_) handle_.resume();}
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

template<typename T>
struct CoroTask
{
	struct CoroTaskPromise
	{
		using handle_t = std::coroutine_handle<CoroTaskPromise>;
		CoroTask get_return_object()
		{
			return CoroTask{ handle_t::from_promise(*this) };
		}

		std::suspend_always initial_suspend() noexcept { return {}; }
		AwaitableFinalContinuation final_suspend() noexcept {
			return {}; 
		}
		
		void unhandled_exception() 
		{
			auto exc = std::current_exception();
			std::rethrow_exception(exc);
		}
		void return_value(T&& val) 
		{
			value = std::forward<T>(val);
		}

		~CoroTaskPromise()
		{
		}

		void set_done_callback(func_t callback) noexcept { done_callback = callback; }

	private:
		friend struct CoroTask;
		friend struct AwaitableFinalContinuation;
		std::coroutine_handle<> continuation = std::noop_coroutine();
		std::coroutine_handle<> handle = std::noop_coroutine();
		func_t done_callback;
		T&& value;
	};

	using promise_type = CoroTaskPromise;
	explicit CoroTask(promise_type::handle_t handle) : handle_(handle) 
	{
		handle.promise().handle = handle_;
	}
	CoroTask(const CoroTask&) = delete;
	CoroTask(CoroTask&& task)
		: handle_(std::exchange(task.handle_, nullptr)) {}
	CoroTask& operator=(const CoroTask&) = delete;
	CoroTask& operator=(CoroTask&& task) {
		handle_ = std::exchange(task.handle_, nullptr);
		return *this;
	}
	
	~CoroTask()
	{
		if (handle_)
			handle_.destroy();
	}
	bool await_ready() noexcept { return false; }
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) {
		handle_.promise().continuation = caller;
		return handle_;
	}

	T&& await_resume() 
	{
		if(handle_)
			return std::move(handle_.promise().value);
	}

	void run() noexcept { if(handle_) handle_.resume();}
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