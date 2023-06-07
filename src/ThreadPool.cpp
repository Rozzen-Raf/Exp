#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threads) : stop(false)
{
    for (size_t i = 0; i < threads; ++i)
        workers.emplace_back(
            [this]
            {
                Run();
            }
            );
}
//---------------------------------------------------------------------

void ThreadPool::Run()
{
    for (;;)
    {
        std::coroutine_handle<> coro;
        {
            lock_t lock(this->queue_mutex);
            this->condition.wait(lock,
                [this] { return this->stop || !this->tasks.empty(); });
            if (this->stop && this->tasks.empty())
                return;
            coro = std::move(this->tasks.front());
            this->tasks.pop();
        }

        coro.resume();
    }
}
//---------------------------------------------------------------------

ThreadPool::~ThreadPool()
{
    {
        lock_t lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers)
        worker.join();
}
//---------------------------------------------------------------------
