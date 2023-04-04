#pragma once
#include "Task.h"
template<typename mutex_t = std::recursive_mutex, typename lock_t = std::lock_guard<mutex_t>>
class Sheduler
{
public:
	Sheduler()
	{
		CurrentThreadId = std::this_thread::get_id();
	}

	void CoroStart(Task&& task)
	{
		//assert(CurrentThreadId == std::this_thread::get_id());
		lock_t lock(mutex);
		auto iter = tasks_map.insert({ task.GetId(), std::move(task) });
		iter.first->second.run(
			[&](Task* task)
			{
				CoroUnreg(task->GetId());
			}
		);
	}

	void CoroUnreg(const UID_t& id)
	{
		//assert(CurrentThreadId == std::this_thread::get_id());
		lock_t lock(mutex);
		auto task = tasks_map.extract(id);
	}
private:
	std::map<UID_t, Task> tasks_map;
	std::thread::id CurrentThreadId;
	mutex_t mutex;
};

