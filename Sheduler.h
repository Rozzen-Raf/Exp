#pragma once
#include "Task.h"
#include "Worker.h"
#include "Awaitable.h"
class Sheduler
{
	using mutex_t = std::recursive_mutex; 
	using lock_t = std::unique_lock<mutex_t>;
private:
	std::unordered_map<UID_t, TaskSharedPtr> tasks_map;
	std::unordered_map<WorkerType, WorkerBaseSharedPtr> Workers;
	mutex_t mutex;
	size_t OwnerThreadID;
	ProcessorSharedPtr Processor;

	struct emit_task
	{
		func_t func;

		func_t GetFunc() const noexcept
		{
			 return func;
		}

		void SetDoneCallback(auto& s) noexcept {}
	};
public:
	Sheduler(ProcessorSharedPtr processor) : Processor(processor)
	{
		std::hash<std::thread::id> hasher;
		OwnerThreadID = hasher(std::this_thread::get_id());
	}

	template<typename T>
	void CoroStart(T&& task)
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

	void CoroUnreg(const UID_t& id)
	{
		lock_t lock(mutex);
		auto task = tasks_map.extract(id);
		int i = task.mapped().use_count();
	}

	void RegisterWorker(WorkerBaseSharedPtr worker)
	{
		std::hash<std::thread::id> hasher;
		assert(OwnerThreadID == hasher(std::this_thread::get_id()));

		using std::placeholders::_1;
		worker->SetEmit(std::bind(&Sheduler::emit, this, _1));
		Workers.insert({ worker->GetType(), worker });
	}

	Awaitable event(WorkerType type, UID_t id)
	{
		auto find_iter = Workers.find(type);
		assert(find_iter != Workers.end());
		return Awaitable(find_iter->second, AwaitableData(type, id));
	}

	void emit(AwaitableData* data)
	{
		auto func = [data]()
		{
			if(data->continuation)
				data->continuation.resume();
		};
		emit_task e{func};

		TaskSharedPtr task = std::make_shared<Task>(std::move(e), Processor);
		task_run(task);
	}
};

