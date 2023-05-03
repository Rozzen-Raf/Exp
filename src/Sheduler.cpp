#include "Sheduler.h"
//-----------------------------------------------------------------
void Sheduler::Run(bool block)
{
    for(auto&& it : Workers)
    {
        auto& coro = it.second.second;

        ShedulerTask e{coro.GetFunc()};

        TaskSharedPtr task = std::make_shared<Task>(std::move(e), Processor);
        task_run(task);
    }

    if(block)
        Processor->Run();
}
//-----------------------------------------------------------------

Sheduler::Sheduler(ProcessorSharedPtr processor) : Processor(processor), AllocationArea(16777216, sizeof(Task))
{
    std::hash<std::thread::id> hasher;
    OwnerThreadID = hasher(std::this_thread::get_id());

    AllocationArea.Init();
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

void Sheduler::CoroUnreg(const UID_t& id)
{
    lock_t lock(mutex);
    tasks_map.extract(id);
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

    auto func = [data]()
    {
        if(data && data->continuation)
            data->continuation.resume();
    };
    ShedulerTask e{func};

    TaskSharedPtr task = make_custom_shared<Task>(AllocationArea, std::move(e), Processor);
    task_run(task);
}
//-----------------------------------------------------------------
