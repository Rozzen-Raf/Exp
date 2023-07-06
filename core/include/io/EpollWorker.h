#include "Worker.h"

namespace io
{
class EpollWorker : public engine::WorkerBase
{
private:
using mutex_t = std::recursive_mutex;
using lock_t = std::unique_lock<mutex_t>;
public:
	enum {Type = engine::EPOLL};
    EpollWorker();

	virtual void RegAwaitable(engine::AwaitableData* data) noexcept final;

	virtual void UnregAwaitable(engine::AwaitableData* data) noexcept final;

	virtual task::CoroTaskVoid Run() final;

	virtual void Stop() noexcept final;

    virtual engine::ID_t GetID() const noexcept final{ return EpollFd;}

	virtual engine::WorkerType GetType() const noexcept final { return static_cast<engine::WorkerType>(Type); }
    int Register(const std::any& fd);

    int Unregister(const std::any& fd);

private:
	std::unordered_map<engine::UID_t, engine::AwaitableData*> Awaitables;
	mutex_t mutex;
	std::atomic<bool> stop;
	int EpollFd = -1;
};
DECLARE_SHARED_PTR(EpollWorker)
}
