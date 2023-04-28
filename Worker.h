#pragma once
#include "Types.h"
#include "ChainableTask.h"
class WorkerBase;
using emitter_f = std::function<void(AwaitableData*, WorkerBase*)> ;

class WorkerBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) noexcept = 0;
	virtual void UnregAwaitable(AwaitableData* data) noexcept = 0;
	virtual WorkerType GetType() const noexcept = 0;
	virtual CoroTaskVoid Run() = 0;
	virtual void Stop() noexcept = 0;
	virtual ID_t GetID() const noexcept = 0;
    virtual int Register(const std::any& fd) = 0;

    virtual int Unregister(const std::any& fd) = 0;

	virtual ~WorkerBase() {}

private:
	friend class Sheduler;
	void set_emit(emitter_f&& emit)
	{
		Emit = std::forward<emitter_f>(emit);
	}
protected:
	emitter_f Emit;
};

typedef std::shared_ptr<WorkerBase> WorkerBaseSharedPtr;
