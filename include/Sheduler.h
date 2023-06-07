#pragma once
#include "Task.h"
#include "Worker.h"
#include "Awaitable.h"
#include "ChainableTask.h"
#include "Allocator.h"

class Sheduler
{
	using mutex_t = std::recursive_mutex; 
    using lock_t = std::unique_lock<mutex_t>;

	struct ShedulerTask
	{
        std::coroutine_handle<> handle;

        std::coroutine_handle<> GetHandle() const noexcept
		{
             return handle;
		}

		void SetDoneCallback(auto& s) noexcept {}
	};
public:
    explicit Sheduler(ProcessorSharedPtr processor);

	void Run(bool block = true);

	void Stop();

	template<typename T>
    void CoroStart(T&& task)
	{
		auto handle = task.GetHandle();
		task.SetDoneCallback([handle, this](){CoroUnreg(handle);});
        task.ControlToSheduler = true;
		Processor->AddTask(handle);
        //return task;
	}

	void CoroUnreg(std::coroutine_handle<> handle);

	void RegisterWorker(WorkerBaseSharedPtr worker);

	Awaitable event(WorkerType type, UID_t id);

	void emit(AwaitableData* data, WorkerBase* worker);
private:
    std::unordered_map<WorkerType, std::pair<WorkerBaseSharedPtr, CoroTaskVoid>> Workers;
    mutex_t mutex;
    size_t OwnerThreadID;
    ProcessorSharedPtr Processor;
};
DECLARE_SHARED_PTR(Sheduler)
DECLARE_WEAK_PTR(Sheduler)
//-----------------------------------------------------------------
