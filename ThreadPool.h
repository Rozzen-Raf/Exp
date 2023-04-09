#pragma once
#include "Concepts.h"
#include "Task.h"
class ThreadPool {
    using mutex_t = std::mutex; 
    using lock_t = std::unique_lock<mutex_t>;
public:
    ThreadPool(size_t threads);

    void Run();

    void AddTask(TaskWithFunc auto task);

    ~ThreadPool();
private:
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;

    mutex_t queue_mutex;
    std::condition_variable condition;
    bool stop;
};
//---------------------------------------------------------------------

void ThreadPool::AddTask(TaskWithFunc auto task)
{
    {
        lock_t lock(queue_mutex);

        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task]() {
            std::invoke(task->GetFunc());
            });
    }
    condition.notify_one();
}
//---------------------------------------------------------------------