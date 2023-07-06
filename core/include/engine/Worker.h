#pragma once
#include "Types.h"
#include "ChainableTask.h"
namespace task{
	class Sheduler;
}

namespace engine
{
class WorkerBase;
struct AwaitableResult
{
	StatusType type = Wait;
    UID_t id;
	String err_message;
	int err;

    constexpr operator bool() const { return type == WakeUp || type == Success;}
};

struct AwaitableData
{
    AwaitableData(WorkerType t, UID_t&& event_id) : EventID(std::move(event_id)), continuation(std::noop_coroutine()), type(t), result{} {}

	UID_t EventID;
	std::coroutine_handle<> continuation;
	AwaitableResult result;
	WorkerType type;
	bool NeedUnreg = true;
	short Events;

	struct event* Event = nullptr;
	WorkerBase* Worker = nullptr;
};

using emitter_f = std::function<void(AwaitableData*)> ;

class WorkerBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) noexcept = 0;
	virtual void UnregAwaitable(AwaitableData* data) noexcept = 0;
	virtual WorkerType GetType() const noexcept = 0;
	virtual task::CoroTaskVoid Run() = 0;
	virtual void Stop() noexcept = 0;
	virtual ID_t GetID() const noexcept = 0;

	virtual ~WorkerBase() {}

    WorkerBase()
    {}

private:
	friend class task::Sheduler;
	void set_emit(emitter_f&& emit)
	{
		Emit = std::forward<emitter_f>(emit);
	}
protected:
	emitter_f Emit;
};

typedef std::shared_ptr<WorkerBase> WorkerBaseSharedPtr;
}