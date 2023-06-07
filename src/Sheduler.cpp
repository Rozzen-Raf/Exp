#include "Sheduler.h"
//-----------------------------------------------------------------
void Sheduler::Run(bool block)
{
    for(auto&& it : Workers)
    {
        auto& coro = it.second.second;

        Processor->AddTask(coro.GetHandle());
    }

    if(block)
        Processor->Run();
}
//-----------------------------------------------------------------

Sheduler::Sheduler(ProcessorSharedPtr processor) : Processor(processor)
{
    std::hash<std::thread::id> hasher;
    OwnerThreadID = hasher(std::this_thread::get_id());
}
//-----------------------------------------------------------------

void Sheduler::Stop()
{
    for(auto&& worker : Workers)
    {
        worker.second.first->Stop();
    }
}
//-----------------------------------------------------------------

void Sheduler::CoroUnreg(std::coroutine_handle<> handle)
{
    if(handle)
    {
        LOG(Sheduler, "Task destroy");
        handle.destroy();
    }
}
//-----------------------------------------------------------------

void Sheduler::RegisterWorker(WorkerBaseSharedPtr worker)
{
    std::hash<std::thread::id> hasher;
    ASSERT(OwnerThreadID == hasher(std::this_thread::get_id()));

    using std::placeholders::_1, std::placeholders::_2;
    worker->set_emit(std::bind(&Sheduler::emit, this, _1, _2));
    auto task = worker->Run();
    auto pair = std::pair<WorkerBaseSharedPtr, CoroTaskVoid>(worker, std::move(task));
    Workers.emplace(worker->GetType(), std::move(pair));
}
//-----------------------------------------------------------------

Awaitable Sheduler::event(WorkerType type, UID_t id)
{
    auto find_iter = Workers.find(type);
    ASSERT(find_iter != Workers.end());
    return Awaitable(find_iter->second.first, AwaitableData(type, std::move(id)), find_iter->second.second.handle_);
}
//-----------------------------------------------------------------

void Sheduler::emit(AwaitableData* data, WorkerBase* worker)
{
    if(data->NeedUnreg)
        worker->UnregAwaitable(data);

      Processor->AddTask(data->continuation);
}
//-----------------------------------------------------------------
