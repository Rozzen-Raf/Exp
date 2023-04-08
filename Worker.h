#pragma once
#include "Types.h"
class WorkerBase;
typedef std::function<void(AwaitableData*, WorkerBase*)> emitter_f;

class WorkerBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) noexcept = 0;
	virtual void UnregAwaitable(AwaitableData* data) noexcept = 0;
	virtual WorkerType GetType() const noexcept = 0;
	virtual void Run() = 0;
	virtual void Stop() noexcept = 0;
	virtual ID_t GetID() const noexcept = 0;

private:
	friend class Sheduler;
	void SetEmit(emitter_f&& emit)
	{
		Emit = std::forward<emitter_f>(emit);
	}
protected:
	emitter_f Emit;
};

typedef std::shared_ptr<WorkerBase> WorkerBaseSharedPtr;