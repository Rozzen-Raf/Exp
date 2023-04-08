#pragma once
#include "Task.h"
#include "Worker.h"
#include "Awaitable.h"
#include "ChainableTask.h"

class Sheduler
{
	using mutex_t = std::recursive_mutex; 
	using lock_t = std::unique_lock<mutex_t>;
private:
	std::unordered_map<UID_t, TaskSharedPtr> tasks_map;
	std::unordered_map<WorkerType, std::pair<WorkerBaseSharedPtr, CoroTaskVoid>> Workers;
	mutex_t mutex;
	size_t OwnerThreadID;
	ProcessorSharedPtr Processor;

	struct ShedulerTask
	{
		func_t func;

		func_t GetFunc() const noexcept
		{
			 return func;
		}

		void SetDoneCallback(auto& s) noexcept {}
	};
public:
	Sheduler(ProcessorSharedPtr processor);

	void Run();

	void Stop();

	template<typename T>
	void CoroStart(T&& task);

	void CoroUnreg(const UID_t& id);

	void RegisterWorker(WorkerBaseSharedPtr worker);

	Awaitable event(WorkerType type, UID_t id);

	void emit(AwaitableData* data, WorkerBase* worker);
};
//-----------------------------------------------------------------
template<typename T>
void Sheduler::CoroStart(T&& task)
{
	lock_t lock(mutex);
	auto task_ptr = std::make_shared<Task>(std::move(task), Processor);
	tasks_map.insert({ task_ptr->GetId(), task_ptr });
	task_run(task_ptr,
		[&](Task* task)
		{
			CoroUnreg(task->GetId());
		}
	);
}
//-----------------------------------------------------------------