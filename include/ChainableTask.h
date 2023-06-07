#pragma once
#include "Types.h"
#include "FinalContinuation.h"

class BrokenPromise : public std::logic_error
{
public:
	BrokenPromise()
		: std::logic_error("broken promise")
	{}
};

template<typename T>
class CoroTask;

class CoroTaskPromiseBase
{
public:	
	std::suspend_always initial_suspend() noexcept { return {}; }

	AwaitableFinalContinuation final_suspend() noexcept {
		return {}; 
	}

	void set_done_callback(func_t callback) noexcept { done_callback = callback; }

	void set_continuation(std::coroutine_handle<> cont) noexcept
	{
		continuation = cont;
	}

	std::coroutine_handle<> get_continuation() const noexcept
	{
		return continuation;
	}
private:
	template <typename T>
	friend class CoroTask;
	friend class AwaitableFinalContinuation;
	std::coroutine_handle<> continuation = std::noop_coroutine();
	func_t done_callback;
};

template<typename T>
class CoroTaskPromise : public CoroTaskPromiseBase
{
public:
	using handle_t = std::coroutine_handle<CoroTaskPromise<T>>;

	CoroTaskPromise() noexcept {}

	~CoroTaskPromise()
	{
		switch (ResultType)
		{
		case result_type::Value:
			Value.~T();
			break;
		case result_type::Exception:
			Exception.~exception_ptr();
			break;
		default:
			break;
		}
	}

	CoroTask<T> get_return_object() noexcept
	{
		return CoroTask<T>{ handle_t::from_promise(*this) };	
	}

	void unhandled_exception() noexcept
	{
		::new (static_cast<void*>(std::addressof(Exception))) std::exception_ptr(
			std::current_exception());
		ResultType = result_type::Exception;
	}

	template<
		typename VALUE,
		typename = std::enable_if_t<std::is_convertible_v<VALUE&&, T>>>
	void return_value(VALUE&& value)
		noexcept(std::is_nothrow_constructible_v<T, VALUE&&>)
	{
		::new (static_cast<void*>(std::addressof(Value))) T(std::forward<VALUE>(value));
		ResultType = result_type::Value;
	}

	T& result() &
	{
		if (ResultType == result_type::Exception)
		{
			std::rethrow_exception(Exception);
		}

		assert(ResultType == result_type::Value);

		return Value;
	}

	using rvalue_type = std::conditional_t<
				std::is_arithmetic_v<T> || std::is_pointer_v<T>,
				T,
				T&&>;

	rvalue_type result() &&
	{
		if (ResultType == result_type::Exception)
		{
			std::rethrow_exception(Exception);
		}

		assert(ResultType == result_type::Value);

		return std::move(Value);
	}

private:
	enum class result_type{Empty, Value, Exception};

	result_type ResultType = result_type::Empty;

	union
	{
		T Value;
		std::exception_ptr Exception;
	};
};

template<>
class CoroTaskPromise<void> : public CoroTaskPromiseBase
{
public:
	using handle_t = std::coroutine_handle<CoroTaskPromise<void>>;

	CoroTaskPromise() noexcept = default;

	CoroTask<void> get_return_object() noexcept;

	void return_void() noexcept
	{}

	void unhandled_exception() noexcept
	{
		Exception = std::current_exception();
	}

	void result()
	{
		if (Exception)
		{
			std::rethrow_exception(Exception);
		}
	}

private:

	std::exception_ptr Exception;

};

template<typename T>
class CoroTask
{
public:
	using promise_type = CoroTaskPromise<T>;
private:
	struct awaitable_base
		{
			std::coroutine_handle<promise_type> m_coroutine;

			awaitable_base(std::coroutine_handle<promise_type> coroutine) noexcept
				: m_coroutine(coroutine)
			{}

			bool await_ready() const noexcept
			{
				return !m_coroutine || m_coroutine.done();
			}

			std::coroutine_handle<> await_suspend(
				std::coroutine_handle<> awaitingCoroutine) noexcept
			{
				m_coroutine.promise().set_continuation(awaitingCoroutine);
				return m_coroutine;
			}
		};
public:

	explicit CoroTask(promise_type::handle_t handle) : handle_(handle) 
	{
	}
	CoroTask(const CoroTask&) = delete;
	CoroTask(CoroTask&& task)
		: handle_(std::exchange(task.handle_, nullptr)) {}

	CoroTask& operator=(const CoroTask&) = delete;
	CoroTask& operator=(CoroTask&& task) {
		if (std::addressof(task) != this)
		{
            if(handle_)
			{
				handle_.destroy();
			}

			handle_ = std::exchange(task.handle_, nullptr);
		}
		return *this;
	}
	
	~CoroTask()
	{
        if (!ControlToSheduler && handle_)
        {
            handle_.destroy();
            handle_ = nullptr;
        }
	}

	auto operator co_await() const & noexcept
	{
		struct awaitable : awaitable_base
		{
			using awaitable_base::awaitable_base;

			decltype(auto) await_resume()
			{
				if (!this->m_coroutine)
				{
					throw BrokenPromise{};
				}

				return this->m_coroutine.promise().result();
			}
		};

		return awaitable{ handle_ };
	}

	auto operator co_await() const && noexcept
	{
		struct awaitable : awaitable_base
		{
			using awaitable_base::awaitable_base;

			decltype(auto) await_resume()
			{
				if (!this->m_coroutine)
				{
					throw BrokenPromise{};
				}

				return std::move(this->m_coroutine.promise()).result();
			}
		};

		return awaitable{ handle_ };
	}

	bool is_ready() const noexcept
	{
		return !handle_ || handle_.done();
	}

	void run() noexcept { if(handle_) handle_.resume();}
private:
	friend class Task;
	friend class Sheduler;

    std::coroutine_handle<> GetHandle() const noexcept {return handle_;};

	void SetDoneCallback(func_t callback)
	{
		if (handle_)
			handle_.promise().set_done_callback(callback);
	}

private:
	promise_type::handle_t handle_;
    bool ControlToSheduler = false;
};

inline CoroTask<void> CoroTaskPromise<void>::get_return_object() noexcept
{
	return CoroTask<void>{ handle_t::from_promise(*this) };
}

using CoroTaskVoid = CoroTask<void>;
