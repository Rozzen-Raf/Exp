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
	std::unordered_map<WorkerType, WorkerBaseSharedPtr> Workers;
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
	Sheduler(ProcessorSharedPtr processor) : Processor(processor)
	{
		std::hash<std::thread::id> hasher;
		OwnerThreadID = hasher(std::this_thread::get_id());
	}

	void Run()
	{
		for(auto&& worker : Workers)
		{
			auto func = [worker]()
			{
				worker.second->Run();
			};
			ShedulerTask e{func};

			TaskSharedPtr task = std::make_shared<Task>(std::move(e), Processor);
			task_run(task);
		}
	}

	void Stop()
	{
		for(auto&& worker : Workers)
		{
			worker.second->Stop();
		}
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

		using std::placeholders::_1, std::placeholders::_2;
		worker->SetEmit(std::bind(&Sheduler::emit, this, _1, _2));
		Workers.insert({ worker->GetType(), worker });
	}

	Awaitable event(WorkerType type, UID_t id)
	{
		auto find_iter = Workers.find(type);
		assert(find_iter != Workers.end());
		return Awaitable(find_iter->second, AwaitableData(type, std::move(id)));
	}

	void emit(AwaitableData* data, WorkerBase* worker)
	{
		worker->UnregAwaitable(data);

		auto func = [data]()
		{
			if(data->continuation)
				data->continuation.resume();
		};
		ShedulerTask e{func};

		TaskSharedPtr task = std::make_shared<Task>(std::move(e), Processor);
		task_run(task);
	}
};

