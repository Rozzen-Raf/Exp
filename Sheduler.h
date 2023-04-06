#pragma once
#include "Task.h"
#include "Worker.h"
#include "Awaitable.h"
template<typename mutex_t = std::recursive_mutex, typename lock_t = std::unique_lock<mutex_t>>
class Sheduler
{
public:
	Sheduler()
	{
		std::hash<std::thread::id> hasher;
		OwnerThreadID = hasher(std::this_thread::get_id());
	}

	void CoroStart(Task&& task)
	{
		lock_t lock(mutex);
		auto task_ptr = std::make_shared< Task>(std::move(task));
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

		Workers.insert({ worker->GetType(), worker });
	}

	Awaitable event(WorkerType type, UID_t id)
	{
		auto find_iter = Workers.find(type);
		if(find_iter == Workers.end())
			return {};
		return Awaitable(AwaitableData(type, id), find_iter->second);
	}
private:
	std::unordered_map<UID_t, TaskSharedPtr> tasks_map;
	std::unordered_map<WorkerType, WorkerBaseSharedPtr> Workers;
	mutex_t mutex;
	size_t OwnerThreadID;
};

