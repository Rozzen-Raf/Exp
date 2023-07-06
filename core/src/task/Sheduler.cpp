#include "Sheduler.h"
namespace task{
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

void Sheduler::RegisterWorker(engine::WorkerBaseSharedPtr worker)
{
    std::hash<std::thread::id> hasher;
    ASSERT(OwnerThreadID == hasher(std::this_thread::get_id()));

    using std::placeholders::_1;
    worker->set_emit(std::bind(&Sheduler::emit, this, _1));
    auto task = worker->Run();
    auto pair = std::pair<engine::WorkerBaseSharedPtr, CoroTaskVoid>(worker, std::move(task));
    Workers.emplace(worker->GetType(), std::move(pair));
}
//-----------------------------------------------------------------

Awaitable Sheduler::event(engine::WorkerType type, engine::UID_t id)
{
    auto find_iter = Workers.find(type);
    ASSERT(find_iter != Workers.end());
    return Awaitable(find_iter->second.first, engine::AwaitableData(type, std::move(id)), find_iter->second.second.handle_);
}
//-----------------------------------------------------------------

void Sheduler::emit(engine::AwaitableData* data)
{
    if(data->NeedUnreg && data->Worker)
        data->Worker->UnregAwaitable(data);

    Processor->AddTask(data->continuation);
}
//-----------------------------------------------------------------
}