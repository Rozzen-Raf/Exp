#pragma once
#include "Task.h"
class Sheduler
{
public:
	Sheduler()
	{
		CurrentThreadId = std::this_thread::get_id();
	}

	void CoroStart(Task&& task)
	{
		assert(CurrentThreadId == std::this_thread::get_id());

		task.run(
			[&](Task* task)
			{
				CoroUnreg(task->GetId());
			}
		);
		tasks_map.insert({ task.GetId(), std::move(task) });
	}

	void CoroUnreg(const UID_t& id)
	{
		assert(CurrentThreadId == std::this_thread::get_id());

		tasks_map.erase(id);
	}
private:
	std::map<UID_t, Task> tasks_map;
	std::thread::id CurrentThreadId;
};

