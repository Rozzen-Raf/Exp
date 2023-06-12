#pragma once
#include "TaskProcessor.h"
#include "Worker.h"
#include "Awaitable.h"
#include "ChainableTask.h"
#include "Allocator.h"

namespace task{
class Sheduler
{
	using mutex_t = std::recursive_mutex; 
    using lock_t = std::unique_lock<mutex_t>;

public:
    explicit Sheduler(ProcessorSharedPtr processor);

	void Run(bool block = true);

	void Stop();

	template<typename T>
    T CoroStart(T&& task)
	{
		auto handle = task.GetHandle();
		task.SetDoneCallback([handle, this](){CoroUnreg(handle);});
        task.ControlToSheduler = true;
		Processor->AddTask(handle);
        return std::move(task);
	}

	void CoroUnreg(std::coroutine_handle<> handle);

	void RegisterWorker(engine::WorkerBaseSharedPtr worker);

	Awaitable event(engine::WorkerType type, engine::UID_t id);

	void emit(engine::AwaitableData* data, engine::WorkerBase* worker);
private:
    std::unordered_map<engine::WorkerType, std::pair<engine::WorkerBaseSharedPtr, CoroTaskVoid>> Workers;
    mutex_t mutex;
    size_t OwnerThreadID;
    ProcessorSharedPtr Processor;
};
DECLARE_SHARED_PTR(Sheduler)
DECLARE_WEAK_PTR(Sheduler)
//-----------------------------------------------------------------
}