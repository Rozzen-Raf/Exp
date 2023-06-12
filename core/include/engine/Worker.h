#pragma once
#include "Types.h"
#include "ChainableTask.h"
namespace task{
	class Sheduler;
}

namespace engine
{
class WorkerBase;
using emitter_f = std::function<void(AwaitableData*, WorkerBase*)> ;
enum WorkFlag{AllAwaiablesEmit, OnlyByID};

class WorkerBase
{
public:
	virtual void RegAwaitable(AwaitableData* data) noexcept = 0;
	virtual void UnregAwaitable(AwaitableData* data) noexcept = 0;
	virtual WorkerType GetType() const noexcept = 0;
	virtual task::CoroTaskVoid Run() = 0;
	virtual void Stop() noexcept = 0;
	virtual ID_t GetID() const noexcept = 0;
    virtual int Register(const std::any& fd) = 0;

    virtual int Unregister(const std::any& fd) = 0;

	virtual ~WorkerBase() {}

    WorkerBase(WorkFlag flag) : Flag(flag)
   {}


private:
	friend class task::Sheduler;
	void set_emit(emitter_f&& emit)
	{
		Emit = std::forward<emitter_f>(emit);
	}
protected:
	emitter_f Emit;
    WorkFlag Flag;
};

typedef std::shared_ptr<WorkerBase> WorkerBaseSharedPtr;
}