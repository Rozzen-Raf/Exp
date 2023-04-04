#pragma once
#include "Task.h"
template<typename mutex_t = std::recursive_mutex, typename lock_t = std::lock_guard<mutex_t>>
class Sheduler
{
public:
	Sheduler()
	{

	}

	void CoroStart(Task&& task)
	{
		lock_t lock(mutex);
		auto task_ptr = std::make_shared< Task>(std::move(task));
		tasks_map.insert({ task.GetId(), task_ptr });
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
	}
private:
	std::map<UID_t, TaskSharedPtr> tasks_map;
	mutex_t mutex;
};

