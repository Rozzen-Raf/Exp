#pragma once
#include "Types.h"
class WorkerBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) noexcept = 0;
	virtual void UnregAwaitable(AwaitableData* data) noexcept = 0;
	virtual WorkerType GetType() const noexcept = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;
};

typedef std::shared_ptr<WorkerBase> WorkerBaseSharedPtr;

template<typename mutex_t = std::recursive_mutex, typename lock_t = std::unique_lock<mutex_t>>
class SelectWorker : public WorkerBase
{
public:
	SelectWorker()
	{
		FD_ZERO(&Desc);
	}
	virtual void RegAwaitable(AwaitableData* data) noexcept final
	{
		if (data->type != GetType())
			return;

		ID_t fd = std::get<ID_t>(data->EventID);
		if(fd == 0)
			return;
		lock_t lock(mutex);
		
		if(Max < fd)
			Max = fd;

		Awaitables.insert({ data->EventID, data });
		FD_SET(fd, &Desc);
	}

	virtual void UnregAwaitable(AwaitableData* data) noexcept
	{
		if(data->type != GetType())
			return;

		ID_t fd = std::get<ID_t>(data->EventID);
		if(fd == 0)
			return;
		lock_t lock(mutex);

		FD_CLR(fd, &Desc);
		Awaitables.erase(data->EventID);
	}

	virtual void Run() final
	{
		stop = false;
		while (!stop)
		{
			int res = select(Max + 1, &Desc, NULL, NULL, NULL);

			if(res == 1)
			{
				for(auto&& await : Awaitables)
				{

				}
			}
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
	std::atomic<bool> stop;
	fd_set Desc;
	int Max = 0;
};

