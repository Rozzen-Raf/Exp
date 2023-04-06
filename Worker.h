#pragma once
#include "Types.h"

typedef std::function<void(AwaitableData*)> emitter_f;

class WorkerBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) noexcept = 0;
	virtual void UnregAwaitable(AwaitableData* data) noexcept = 0;
	virtual WorkerType GetType() const noexcept = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;

private:
	friend class Sheduler;
	void SetEmit(emitter_f&& emit)
	{
		Emit = std::forward<emitter_f>(emit);
	}

	emitter_f Emit;
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

		if(!std::holds_alternative<ID_t>(data->EventID));
			return;

		ID_t fd = std::get<ID_t>(data->EventID);
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

		if(!std::holds_alternative<ID_t>(data->EventID));
			return;

		ID_t fd = std::get<ID_t>(data->EventID);

		lock_t lock(mutex);

		FD_CLR(fd, &Desc);
		Awaitables.erase(data->EventID);
	}

	virtual void Run() final
	{
		stop = false;
		while (!stop)
		{
			fd_set select_set = Desc;
			int res = select(Max + 1, &select_set, NULL, NULL, NULL);

			if(res == 1)
			{
				lock_t lock(mutex);
				for(auto&& await : Awaitables)
				{
					ID_t fd = std::get<int>(await.first);

					if(FD_ISSET(fd, &select_set) && Emit)
					{
						Emit(await.second);
					}
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


private:
	std::unordered_map<UID_t, AwaitableData*> Awaitables;
	mutex_t mutex;
	std::atomic<bool> stop;
	fd_set Desc;
	int Max = 0;
};

