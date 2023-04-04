#include "ThreadPool.h"

void ThreadPool::AddTask(func_t func)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace(func);
    }
    condition.notify_one();
}
