#pragma once
#include "Types.h"
class WorkerBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) noexcept = 0;
	virtual WorkerType GetType() const noexcept = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;
};

typedef std::shared_ptr<WorkerBase> WorkerBaseSharedPtr;

template<typename mutex_t = std::recursive_mutex, typename lock_t = std::unique_lock<mutex_t>>
class SelectWorker : public WorkerBase
{
public:

	virtual void RegAwaitable(AwaitableData* data) noexcept final
	{
		if (data->type != WorkerType::SELECT)
			return;
		lock_t lock(mutex);
		Awaitables->insert({ data->EventID, data });
	}

	virtual void Run() final
	{
		stop = false;
		while (!stop)
		{

		}
	}

	virtual void Stop() final
	{
		stop = true;
	}

	virtual WorkerType GetType() const noexcept final { return WorkerType::SELECT; }
private:
	std::unordered_map<UID_t, AwaitableData*> Awaitables;
	mutex_t mutex;
	std::atomic_flag stop;
};

