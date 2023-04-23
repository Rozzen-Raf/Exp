#include "Worker.h"

class EpollWorker : public WorkerBase
{
private:
using mutex_t = std::recursive_mutex;
using lock_t = std::unique_lock<mutex_t>;
public:
	EpollWorker();

	virtual void RegAwaitable(AwaitableData* data) noexcept final;

	virtual void UnregAwaitable(AwaitableData* data) noexcept final;

	virtual CoroTaskVoid Run() final;

	virtual void Stop() noexcept final;

    virtual ID_t GetID() const noexcept final{ return EpollFd;}

	virtual WorkerType GetType() const noexcept final { return WorkerType::EPOLL; }

    virtual int Register(int fd) final;

    virtual int Unregister(int fd) final;

private:
	std::unordered_map<UID_t, AwaitableData*> Awaitables;
	mutex_t mutex;
	std::atomic<bool> stop;
	int EpollFd = -1;
};

